#include<opencv2/opencv.hpp>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
using namespace cv;
using namespace std;

//----------------------------convolution function-------------------------------------
/////////////////////////////convolution parameter 입력
int conv_inputdata(int* W_row,int* W_col, int* F, int* P, int* S,int* C, int* resultsize_row,int* resultsize_col,int* f_select)
{

    printf("Image Size : %d x %d \n",*W_row,*W_col);
//    printf("input Filter Size : ");
//    scanf("%d",F);
    
    printf("input Padding Size : ");
    scanf("%d",P);
    printf("input Stride : ");
    scanf("%d",S);
    printf("Channel num : ");
    scanf("%d",C);
    printf("-----------select filter---------\n");
    printf("1. 3x3 Blur / 2. 5x5 Blur / 3. Edge detector / 4. Emboss / 5. Outline\n");
    scanf("%d",f_select);
    if(*f_select==1){*F=3;}
    else if(*f_select==2){*F=5;}
    else if(*f_select==3){*F=3;}
    else if(*f_select==4){*F=3;}
    else if(*f_select==5){*F=3;}

    *resultsize_row = (((*W_row)-(*F)+(2*(*P)))/(*S)) + 1;
    *resultsize_col = (((*W_col)-(*F)+(2*(*P)))/(*S)) + 1;
}

//////////////////////padding///////////////////////////
int padding(int* W_row,int* W_col, int* F, int* P, int* S,int* C, int* resultsize_row, int* resultsize_col, Mat image, int**** p1, float*** f1, int**** r1,int* f_select)
{
//image matrix malloc(padding)
    (*p1) = (int***)malloc((*C)*sizeof(int**));
    for(int i=0; i<(*C); i++)
    {
        (*p1)[i] = (int**)malloc((*W_row+ 2*(*P))*sizeof(int*));
        for(int j=0; j<(*W_row+2*(*P)); j++)
        {
            (*p1)[i][j] = (int*)malloc((*W_col+2*(*P))*sizeof(int));
        }
    }
//padding matrix init
    for(int i=0;i<(*W_row+2*(*P));i++)
	{
		for(int j=0;j<(*W_col+2*(*P));j++)
		{
            for(int k=0;k<(*C);k++)
            {
                (*p1)[k][i][j] = 0;
            }
        }
    }


//filter malloc
    (*f1) = (float**)malloc(*F*sizeof(float*));
    for(int i=0; i<(*F); i++)
    {
        (*f1)[i] = (float*)malloc(*F*sizeof(float));
    }
//filter init
    for(int i=0;i<(*F);i++)
	{
		for(int j=0;j<(*F);j++)
		{
			(*f1)[i][j] = 0;
		}
	}

    if(*f_select==1) // 3x3 blur
    {
        (*f1)[0][0] = 1.0/16.0; 
        (*f1)[0][1] = 2.0/16.0; 
        (*f1)[0][2] = 1.0/16.0;
        (*f1)[1][0] = 2.0/16.0; 
        (*f1)[1][1] = 4.0/16.0; 
        (*f1)[1][2] = 2.0/16.0; 
        (*f1)[2][0] = 1.0/16.0; 
        (*f1)[2][1] = 2.0/16.0; 
        (*f1)[2][2] = 1.0/16.0; 
    }

    else if(*f_select==2) // 5x5 blur
    {
        (*f1)[0][0] = 1.0/256.0; 
        (*f1)[0][1] = 4.0/256.0; 
        (*f1)[0][2] = 6.0/256.0;
        (*f1)[0][3] = 4.0/256.0; 
        (*f1)[0][4] = 1.0/256.0; 
        (*f1)[1][0] = 4.0/256.0; 
        (*f1)[1][1] = 16.0/256.0; 
        (*f1)[1][2] = 24.0/256.0;
        (*f1)[1][3] = 16.0/256.0; 
        (*f1)[1][4] = 4.0/256.0; 
        (*f1)[2][0] = 6.0/256.0; 
        (*f1)[2][1] = 24.0/256.0; 
        (*f1)[2][2] = 36.0/256.0;
        (*f1)[2][3] = 24.0/256.0; 
        (*f1)[2][4] = 6.0/256.0; 
        (*f1)[3][0] = 4.0/256.0; 
        (*f1)[3][1] = 16.0/256.0; 
        (*f1)[3][2] = 24.0/256.0;
        (*f1)[3][3] = 16.0/256.0; 
        (*f1)[3][4] = 4.0/256.0; 
        (*f1)[4][0] = 1.0/256.0; 
        (*f1)[4][1] = 4.0/256.0; 
        (*f1)[4][2] = 6.0/256.0;
        (*f1)[4][3] = 4.0/256.0; 
        (*f1)[4][4] = 1.0/256.0; 
    }

    else if(*f_select==3) // Edge detector
    {
        (*f1)[0][0] = -1.0; 
        (*f1)[0][1] = -1.0; 
        (*f1)[0][2] = -1.0;
        (*f1)[1][0] = -1.0;  
        (*f1)[1][1] = 8.0; 
        (*f1)[1][2] = -1.0; 
        (*f1)[2][0] = -1.0;
        (*f1)[2][1] = -1.0; 
        (*f1)[2][2] = -1.0; 
    }

    else if(*f_select==4) ///Emboss
    {
        (*f1)[0][0] = 1.0; 
        (*f1)[0][1] = 1.0; 
        (*f1)[0][2] = 0.0;
        (*f1)[1][0] = 1.0;  
        (*f1)[1][1] = 0.0; 
        (*f1)[1][2] = -1.0; 
        (*f1)[2][0] = 0.0;
        (*f1)[2][1] = -1.0; 
        (*f1)[2][2] = -1.0; 
    }

    else if(*f_select==5) ///Outline 
    {
        (*f1)[0][0] = 1.0; 
        (*f1)[0][1] = 1.0; 
        (*f1)[0][2] = 1.0;
        (*f1)[1][0] = 1.0;  
        (*f1)[1][1] = -7.0; 
        (*f1)[1][2] = 1.0; 
        (*f1)[2][0] = 1.0;
        (*f1)[2][1] = 1.0; 
        (*f1)[2][2] = 1.0; 
    }

    printf("%d x %d Filter\n",*F,*F);
    for(int i=0;i<(*F);i++)
	{
		for(int j=0;j<(*F);j++)
		{
			printf(" %3f ",(*f1)[i][j]);
		}
        printf("\n");
	}
    printf("\n");
//result matrix malloc
    (*r1) = (int***)malloc((*C)*sizeof(int**));
    for(int i=0; i<(*C); i++)
    {
        (*r1)[i] = (int**)malloc(*resultsize_row*sizeof(int*));
        for(int j=0; j<*resultsize_row; j++)
        {
            (*r1)[i][j] = (int*)malloc(*resultsize_col*sizeof(int));
        }
    }
//result matrix init    
    for(int k=0;k<(*C);k++)
    {
        for(int i=0;i<(*resultsize_row);i++)
        {
            for(int j=0;j<(*resultsize_col);j++)
            {
                (*r1)[k][i][j] = 0;
            }
        }
    }
//////////////image padding
    for(int x=0;x<(*W_row+2*(*P));x++)
	{
		for(int y=0;y<(*W_col+2*(*P));y++)
		{
            for(int z=0;z<(*C);z++)
            {
                if(x <= *P-1 || y <= *P-1 || x >= *W_row+*P || y>= *W_col+*P)
                {
                    (*p1)[z][x][y] = 0;
                }
                else
                {
                    Vec3b* pixel = image.ptr<Vec3b>(x-*P);
                    (*p1)[z][x][y] = pixel[y-*P][z];
                }
            }
		}
	}
}

/////////////////////////convolution ////////////////////////////////
int convolution(int* W_row, int* W_col, int* F, int* P, int* S,int* C,int* resultsize_row, int* resultsize_col,int**** p1,float*** f1,int**** r1)
{
   clock_t begin = clock(); //time check
   for(int k=0;k<*C;k++)
   {
       for(int m=0;m<*resultsize_row;m++)
       {
           for(int n=0;n<*resultsize_col;n++)
           {
               for(int i=0;i<*F;i++)
               {
                   for(int j=0;j<*F;j++)
                   {
                       (*r1)[k][m][n] +=  (*p1)[k][i+m*(*S)][j+n*(*S)] * (*f1)[i][j];
                   }
               }
           }
       }
   }
   clock_t end = clock();  //time check
   double elapsed_secs = double(end-begin);
   printf("\n\n convolution time : %lf\n\n",elapsed_secs);
//saturation
    for(int k=0;k<(*C);k++)
    {
        for(int i=0;i<*resultsize_row;i++)
        {
            for(int j=0;j<*resultsize_col;j++)
            {
                if(((*r1)[k][i][j]) > 255)
                {
                    ((*r1)[k][i][j]) = 255;
                }
                else if(((*r1)[k][i][j]) < 0)
                {
                    ((*r1)[k][i][j]) = 0;
                }
                else
                {
                     ((*r1)[k][i][j]) =  ((*r1)[k][i][j]);
                }
                
            }
        }
    }
}

///////////////////////////////////////save image///////////////////////////////////
int saveimage(int* W_row, int* W_col,int* F,int* P,int* S,int* C,int* resultsize_row, int* resultsize_col,int**** r1,Mat image)
{
    Mat image_out(*resultsize_row,*resultsize_col,image.type());
    for(int x=0;x<*resultsize_row;x++)
	{
		for(int y=0;y<*resultsize_col;y++)
		{
            for(int z=0;z<*C;z++)
            {
                cv::Vec3b pixel = image_out.at<cv::Vec3b>(x,y);
                image_out.at<cv::Vec3b>(x,y)[z] = (*r1)[z][x][y];
            }
        }
    }
    namedWindow("Original image",WINDOW_AUTOSIZE);
    imshow("Original image",image);
    namedWindow("Convolution image",WINDOW_AUTOSIZE);
    imshow("Convolution image",image_out);

    waitKey(0);
    //imwrite("after4.jpg",image_out);
    //printf("complete save\n");
}



//----------------------------Pooling function-------------------------------------
/////////////////////////////pooling parameter 입력
int pooling_inputdata(int* p_W_row,int* p_W_col, int* p_F, int* p_S,int* p_C, int* p_resultsize_row, int* p_resultsize_col)
{
    printf("Image Size : %d x %d \n",*p_W_row,*p_W_col);
    printf("input Filter Size : ");
    scanf("%d",p_F);
    printf("input Stride : ");
    scanf("%d",p_S);
    printf("Channel num : ");
    scanf("%d",p_C);

    *p_resultsize_row = ((*p_W_row-*p_F)/ *p_S) + 1;
    *p_resultsize_col = ((*p_W_col-*p_F)/ *p_S) + 1;
}
/////////////////////////////maxpooling
int maxpooling(int* p_W_row,int* p_W_col, int* p_F, int* p_S,int* p_C, int* p_resultsize_row, int* p_resultsize_col, int**** p2, int**** r2,Mat image)
{
//image matrix malloc
    (*p2) = (int***)malloc((*p_C)*sizeof(int**));
    for(int i=0; i<(*p_C); i++)
    {
        (*p2)[i] = (int**)malloc(*p_W_row*sizeof(int*));
        for(int j=0; j<*p_W_row; j++)
        {
            (*p2)[i][j] = (int*)malloc(*p_W_col*sizeof(int));
        }
    }
//image matrix init
    for(int i=0;i<*p_W_row;i++)
	{
		for(int j=0;j<*p_W_col;j++)
		{
            for(int k=0;k<*p_C;k++)
            {
                (*p2)[k][i][j] = 0;
            }
        }
    }

//pooling result matrix malloc
    (*r2) = (int***)malloc((*p_C)*sizeof(int**));
    for(int i=0; i<(*p_C); i++)
    {
        (*r2)[i] = (int**)malloc(*p_resultsize_row* sizeof(int*));
        for(int j=0; j<*p_resultsize_row; j++)
        {
            (*r2)[i][j] = (int*)malloc(*p_resultsize_col*sizeof(int));
        }
    }
//pooling result matrix init
    for(int k=0;k<(*p_C);k++)
    {
        for(int i=0;i<(*p_resultsize_row);i++)
        {
            for(int j=0;j<(*p_resultsize_col);j++)
            {
                (*r2)[k][i][j] = 0;
            }
        }
    }

//image matrix
    for(int x=0;x<*p_W_row;x++)
	{
		for(int y=0;y<*p_W_col;y++)
		{
            for(int z=0;z<*p_C;z++)
            {
                Vec3b* pixel = image.ptr<Vec3b>(x); //////////제대로 이미지값 들어가는지 확인해볼것!!!!!!
                (*p2)[z][x][y] = pixel[y][z]; //////////////제대로 이미지값 들어가는지 확인해볼것!!!!!!!
            }
		}
	}

////////////////////////maxpooling
    int max;
    for(int k=0;k<*p_C;k++)
    {
        for(int x=0;x<*p_resultsize_row;x++)
        {
            for(int y=0;y<*p_resultsize_col;y++)
            {
                max = 0;
                for(int i=0;i<*p_F;i++)
                {
                    for(int j=0;j<*p_F;j++)
                    {
                        if(max < (*p2)[k][i+x*(*p_S)][j+y*(*p_S)])
                        {
                            max = (*p2)[k][i+x*(*p_S)][j+y*(*p_S)];
                        }
                    }
                }
                (*r2)[k][x][y] = max;
            }
        }
    }

//image save
    Mat image_out2(*p_resultsize_row,*p_resultsize_col,image.type());
    for(int x=0;x<*p_resultsize_row;x++)
	{
		for(int y=0;y<*p_resultsize_col;y++)
		{
            for(int z=0;z<*p_C;z++)
            {
                
                cv::Vec3b pixel = image_out2.at<cv::Vec3b>(x,y);
                image_out2.at<cv::Vec3b>(x,y)[z] = (*r2)[z][x][y];
                
            }
        }
    }

    namedWindow("Original image",WINDOW_AUTOSIZE);
    imshow("Original image",image);
    namedWindow("Pooling image",WINDOW_AUTOSIZE);
    imshow("Pooling image",image_out2);

    waitKey(0);
   // imwrite("pooling.jpg",image_out2);
   // printf("save pooling image\n");

}



int main()
{
    Mat image;
    image = imread("son.jpg",IMREAD_COLOR);
    if(image.empty())
    {
        cout << "Could not open or find the image" << endl;
        return -1;
    }
//convolution parameter
    int f_select=0;
    int W_row; ///image size
    int W_col;
    int F; // Filter size F x F
    int P; // Padding
    int S; // Stride
    int C; // channel
    int resultsize_row; //result size
    int resultsize_col;

    W_row = image.rows;
    W_col = image.cols;
    int ***p1 = NULL; //image(padding)
    float **f1 = NULL; //convolution filter
    int ***r1 = NULL; //convolution result
//pooling parameter
    int p_W_row;//image size W x W
    int p_W_col;
    int p_F;//pooling filter size
    int p_S;//pooling stride
    int p_C;//channel
    int p_resultsize_row; //after pooling resultsize
    int p_resultsize_col;

    p_W_row = image.rows;
    p_W_col = image.cols;
    int ***p2 = NULL; //image(not padding)
    int ***r2 = NULL; // pooling result

    int select=0;
    printf(" 1.convolution   /   2.pooling \n");
    scanf("%d",&select);
    if(select==1)
    {
        conv_inputdata(&W_row,&W_col,&F,&P,&S,&C,&resultsize_row,&resultsize_col,&f_select);
        padding(&W_row,&W_col,&F,&P,&S,&C,&resultsize_row,&resultsize_col,image, &p1,&f1,&r1,&f_select);
        convolution(&W_row,&W_col,&F,&P,&S,&C,&resultsize_row,&resultsize_col,&p1,&f1,&r1);
        saveimage(&W_row,&W_col,&F,&P,&S,&C,&resultsize_row,&resultsize_col,&r1,image);

        for(int i=0; i<C;i++)
        {
            for(int j=0; j<W_row+2*P; j++)
            {
                free(*(*(p1+i)+j));
            }
            free(*(p1+i));
        }
        free(p1);

        for(int i=0; i<F; i++)
        {
            free(*(f1+i));
        }
        free(f1);

        for(int i=0; i<C;i++)
        {
            for(int j=0; j<resultsize_row; j++)
            {
                free(*(*(r1+i)+j));
            }
            free(*(r1+i));
        }
        free(r1);
    }

    else if(select==2)
    {
        pooling_inputdata(&p_W_row,&p_W_col,&p_F,&p_S,&p_C,&p_resultsize_row,&p_resultsize_col);
        maxpooling(&p_W_row,&p_W_col,&p_F,&p_S,&p_C,&p_resultsize_row,&p_resultsize_col,&p2,&r2,image);

        for(int i=0; i<p_C;i++)
        {
            for(int j=0; j<p_W_row; j++)
            {
                free(*(*(p2+i)+j));
            }
            free(*(p2+i));
        }
        free(p2);

        for(int i=0; i<p_C;i++)
        {
            for(int j=0; j<p_resultsize_row; j++)
            {
                free(*(*(r2+i)+j));
            }
            free(*(r2+i));
        }
        free(r2);
    }
}


//Activation function
/*
    double getActivation(int****r1, int* C, int* resultsize_row, int* resultsize_col, int* F, const double sigma)
    {
        int ***output=NULL;

        output = (int***)malloc((*C)*sizeof(int**));
         for(int i=0; i<(*C); i++)
        {
            output[i] = (int**)malloc(*resultsize_row*sizeof(int*));
            for(int j=0; j<*resultsize_row; j++)
            {
                output[i][j] = (int*)malloc(*resultsize_col*sizeof(int));
            }
        }


        for(int k=0; k < *C; k++){
            for(int i=0; i < *resultsize_row; i++){
                for(int j=0; j < *resultsize_col; j++)
                {
                  output[k][i][j] =  1/(1+exp(-(*r1)[k][i][j]));
                }
            }
        }
    }
*/