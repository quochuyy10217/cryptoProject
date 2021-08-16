
# Cryptography Project: Images Homomorphic Encryption using Paillier

Encrypt images through Homomorphic cryptosystem to ensure that third parties cannot know what the original data is even if they are authorized to process the image

An excutable program after compiling the cpp file can encrypt and decrypt image(.png, .jpg) using Paillier cryptosystem.
## Authors

- [@quochuyy10217](https://github.com/quochuyy10217)
- [@trisngo](https://github.com/trisngo)
  
## Installation

This program can only run on Linux operating system. You must install some required library to make sure the program run properly:

- Gmp library (https://gmplib.org/)
- libhcs (https://github.com/tiehuis/libhcs)
- opencv4 (https://github.com/opencv/opencv)

If you have already installed all required libraries, you can build the program with command below:

```bash
  g++ filename.cpp `pkg-config --libs opencv4` -o filename -lhcs -lgmp -std=c++11 -pthread
```
    
## Appendix

This is a university cryptography project for which we consulted a number of sources. We successfully tested it, but we are not responsible for any bugs.

  
## References

1. Wade, M. I., Chouikha, M., Gill, T., Patterson, W., Washington, T. M., & Zeng, J. (2019, 10-12 Oct. 2019). Distributed Image Encryption Based On a Homomorphic Cryptographic Approach. 2019 IEEE 10th Annual Ubiquitous Computing, Electronics & Mobile Communication Conference (UEMCON).
2. Mazza, S., Patel, D., & Viola, I. (2021). Homomorphic-Encrypted Volume Rendering. IEEE Transactions on Visualization and Computer Graphics, 27(2), 635-644. https://doi.org/10.1109/TVCG.2020.3030436 
3. Qin, Z., Weng, J., Cui, Y., & Ren, K. (2018). Privacy-Preserving Image Processing in the Cloud. IEEE Cloud Computing, 5(2), 48-57. https://doi.org/10.1109/MCC.2018.022171667 
4. Fu, W., Lin, R., & Inge, D. (2018). Fully Homomorphic Image Processing. CoRR, abs/1810.03249. http://arxiv.org/abs/1810.03249
5. Shah, M., Zhang, W., Hu, H., & Yu, N. (2019). Paillier Cryptosystem based Mean Value Computation for Encrypted Domain Image Processing Operations. ACM Trans. Multimedia Comput. Commun. Appl., 15(3), Article 76. https://doi.org/10.1145/3325194
