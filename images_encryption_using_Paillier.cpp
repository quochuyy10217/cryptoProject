//Building file using this command: g++ filename.cpp `pkg-config --libs opencv4` -o filename -lhcs -lgmp -std=c++11 -pthread
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <gmp.h>    
#include <libhcs.h> 
#include <fstream>  
#include <thread>   
#include <future>   
#include <unistd.h> 
#include <boost/algorithm/string.hpp>
using namespace std;
using namespace cv;


void paillier_encryption(Mat plainMat,Mat &destinationMat, pcs_public_key* pk, pcs_private_key*vk, hcs_random* hr, string &qt);
void paillier_decryption(Mat img1, Mat img2, Mat &originalMat,vector<string> quotes1, vector<string> quotes2, pcs_public_key* pk, pcs_private_key* vk);

int main()
{
    string fileName;
    cout<<"Enter name of an image you want to encrypt (an image and this program must be in the same directory, *.jpg or *.png): "<<endl;
    cin>>fileName;
    char* temp = get_current_dir_name();
    string filePath = temp;
    filePath = filePath + "/" + fileName;
    int keySize;
    cout<<"Enter the length of the key (bits):"<<endl;
    cin>>keySize;
    cout<<"Start encryption..."<<endl;

    auto start = chrono::steady_clock::now();
    Mat original_img = imread(filePath);
    Mat bgr_channel[3];
    split(original_img,bgr_channel);
    Mat components_Image[6];


    components_Image[0] = bgr_channel[0].clone(); //Blue_1
    components_Image[1] = bgr_channel[0].clone(); //Blue_2
    components_Image[2] = bgr_channel[1].clone(); //Green_1
    components_Image[3] = bgr_channel[1].clone(); //Green_2
    components_Image[4] = bgr_channel[2].clone(); //Red_1
    components_Image[5] = bgr_channel[2].clone(); //Red_2
    
    for (int i=0;i<original_img.rows;i++)
        for (int j=0;j<original_img.cols;j++)
        {
            int temp;
            if (bgr_channel[0].at<uchar>(i,j)!=0)
            {
                temp = 0 + rand() % (( bgr_channel[0].at<uchar>(i,j) + 1 ) - 0);
                components_Image[0].at<uchar>(i,j) = temp;
                components_Image[1].at<uchar>(i,j) = bgr_channel[0].at<uchar>(i,j) - temp;
            } else if (bgr_channel[0].at<uchar>(i,j) ==0)
            {
                components_Image[0].at<uchar>(i,j) = 0;
                components_Image[1].at<uchar>(i,j) = 0;
            }
            if (bgr_channel[1].at<uchar>(i,j)!=0)
            {
                temp = 0 + rand() % (( bgr_channel[1].at<uchar>(i,j) + 1 ) - 0);
                components_Image[2].at<uchar>(i,j) = temp;
                components_Image[3].at<uchar>(i,j) = bgr_channel[1].at<uchar>(i,j) - temp;
            }
            else if (bgr_channel[0].at<uchar>(i,j) ==0)
            {
                components_Image[2].at<uchar>(i,j) = 0;
                components_Image[3].at<uchar>(i,j) = 0;
            }
            if (bgr_channel[2].at<uchar>(i,j)!=0)
            {
                temp = 0 + rand() % (( bgr_channel[2].at<uchar>(i,j) + 1 ) - 0);
                components_Image[4].at<uchar>(i,j) = temp;
                components_Image[5].at<uchar>(i,j) = bgr_channel[2].at<uchar>(i,j) - temp;
            } else if (bgr_channel[0].at<uchar>(i,j) ==0)
            {
                components_Image[4].at<uchar>(i,j) = 0;
                components_Image[5].at<uchar>(i,j) = 0;
            }
        }
    
    Mat encrypted_image[6];

    string qt[6];
    for (int i=0;i<6;i++)
    {
        qt[i] = "";
    }

    pcs_public_key* pk = pcs_init_public_key();
    pcs_private_key* vk = pcs_init_private_key();
    hcs_random* hr = hcs_init_random();
    pcs_generate_key_pair(pk, vk, hr, keySize);


    future<void> t1 = async(paillier_encryption,components_Image[0],ref(encrypted_image[0]),pk,vk,hr,ref(qt[0]));        
    future<void> t2 = async(paillier_encryption,components_Image[1],ref(encrypted_image[1]),pk,vk,hr,ref(qt[1]));
    future<void> t3 = async(paillier_encryption,components_Image[2],ref(encrypted_image[2]),pk,vk,hr,ref(qt[2]));
    future<void> t4 = async(paillier_encryption,components_Image[3],ref(encrypted_image[3]),pk,vk,hr,ref(qt[3])); 
    future<void> t5 = async(paillier_encryption,components_Image[4],ref(encrypted_image[4]),pk,vk,hr,ref(qt[4]));
    future<void> t6 = async(paillier_encryption,components_Image[5],ref(encrypted_image[5]),pk,vk,hr,ref(qt[5]));  
    t1.get();
    t2.get();
    t3.get();
    t4.get();
    t5.get();
    t6.get();   
    
    imwrite("B1.png",encrypted_image[0]);
    imwrite("B2.png",encrypted_image[1]);
    imwrite("G1.png",encrypted_image[2]);
    imwrite("G2.png",encrypted_image[3]);
    imwrite("R1.png",encrypted_image[4]);
    imwrite("R2.png",encrypted_image[5]);


    auto end = chrono::steady_clock::now();
    cout <<"Encryption time is: ";
    cout << chrono::duration <double> (end-start).count() << "s" << endl;
    
    string contiCheck = "";
    cout << "Do you want to continue the decryption phase?(y/n)";
    cin >> contiCheck;
    if(contiCheck == "n" || contiCheck == "N" || contiCheck == "no" || contiCheck == "NO")
        return 0;

    //Decryptiion
    auto start_decrypt = chrono::steady_clock::now();

    cout<<"Start Decryption..."<<endl;
    
    vector<string> quotesPart0;
    vector<string> quotesPart1;
    vector<string> quotesPart2;
    vector<string> quotesPart3;
    vector<string> quotesPart4;
    vector<string> quotesPart5;

    boost::split(quotesPart0, qt[0], boost::is_any_of(";"));
    boost::split(quotesPart1, qt[1], boost::is_any_of(";"));
    boost::split(quotesPart2, qt[2], boost::is_any_of(";"));
    boost::split(quotesPart3, qt[3], boost::is_any_of(";"));
    boost::split(quotesPart4, qt[4], boost::is_any_of(";"));
    boost::split(quotesPart5, qt[5], boost::is_any_of(";"));

    Mat recoveredImg[3];
    recoveredImg[0] = encrypted_image[0].clone();
    recoveredImg[1] = encrypted_image[0].clone();
    recoveredImg[2] = encrypted_image[0].clone();

    future<void> t7 = async(paillier_decryption,encrypted_image[0],encrypted_image[1],ref(recoveredImg[0]),quotesPart0,quotesPart1,pk,vk);
    future<void> t8 = async(paillier_decryption,encrypted_image[2],encrypted_image[3],ref(recoveredImg[1]),quotesPart2,quotesPart3,pk,vk); 
    future<void> t9 = async(paillier_decryption,encrypted_image[4],encrypted_image[5],ref(recoveredImg[2]),quotesPart4,quotesPart5,pk,vk);  

    t7.get();
    t8.get();
    t9.get();

    Mat finalResult;
    vector<Mat> vectorOfRecovered;
    for (int i=0;i<3;i++)
    {
        vectorOfRecovered.push_back(recoveredImg[i]);
    }

    merge(vectorOfRecovered,finalResult);
    imwrite("RecoveredImage.jpg",finalResult);
    auto end_decrypt = chrono::steady_clock::now();

    cout<<"Decryption time is: ";
    cout<<chrono::duration <double> (end_decrypt-start_decrypt).count() << "s" << endl;

    pcs_free_private_key(vk);
    pcs_free_public_key(pk);


    return 0;
}

void paillier_encryption(Mat plainMat,Mat &destinationMat, pcs_public_key* pk, pcs_private_key*vk, hcs_random* hr, string &qt)
{
    destinationMat = plainMat.clone();
    for (int i=0;i<plainMat.rows;i++)
        for (int j=0;j<plainMat.cols;j++)
        {
            while (true)
            {
                bool break_flag = false;     
                mpz_t encryptedValue,newValue,modNumber,thuongSo;
                mpz_inits(encryptedValue,newValue,modNumber,thuongSo,NULL);
                mpz_set_ui(encryptedValue,plainMat.at<uchar>(i,j));
                mpz_set_ui(newValue,0);
                mpz_set_ui(modNumber,257);
                mpz_set_ui(thuongSo,0);
                pcs_encrypt(pk,hr,encryptedValue,encryptedValue);
                mpz_mod(newValue,encryptedValue,modNumber);
                mpz_t decryptedValue;
                mpz_init(decryptedValue);
                if (mpz_cmp_ui(newValue,255)<0) 
                {
                    pcs_decrypt(vk,decryptedValue,encryptedValue);
                    mpz_fdiv_q_ui(thuongSo,encryptedValue,257);                    
                    destinationMat.at<uchar>(i,j) = mpz_get_ui(newValue);
                    break_flag = true;
                    char * charThuongSo = 0;
                    charThuongSo = mpz_get_str(charThuongSo,10,thuongSo);
                    string strThuongSo = charThuongSo;
                    qt += strThuongSo + ";"; 
                    break_flag = true;
                } 
                mpz_clears(encryptedValue,newValue,modNumber,thuongSo, NULL);
                if (break_flag==true) break;
            } 
        }
}

void paillier_decryption(Mat img1, Mat img2, Mat &destinationMat,vector<string> quotes1, vector<string> quotes2, pcs_public_key* pk, pcs_private_key* vk)
{
    int index = 0;
    for (int i=0;i<destinationMat.rows;i++)
        for (int j=0;j<destinationMat.cols;j++)
        {
            mpz_t number1, number2, result;
            const char * c1 = quotes1[index].c_str();
            const char * c2 = quotes2[index].c_str();
            mpz_init(number1);
            mpz_init(number2);
            mpz_init(result);
            mpz_set_str(number1,c1,10);
            mpz_set_str(number2,c2,10);
            mpz_mul_ui(number1,number1,257);
            mpz_mul_ui(number2,number2,257);
            mpz_add_ui(number1,number1,img1.at<uchar>(i,j));
            mpz_add_ui(number2,number2,img2.at<uchar>(i,j));
            pcs_ee_add(pk,result,number1,number2);
            pcs_decrypt(vk,result,result);
            unsigned long int recoveredValue =  mpz_get_ui (result);
            destinationMat.at<uchar>(i,j) = recoveredValue;
            index++;
        }
}