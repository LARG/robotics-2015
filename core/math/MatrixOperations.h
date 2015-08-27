#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include <iostream>
#include <Eigen/Core>
#define PRINTMAT(x) std::cout << #x << ":\n" << x << "\n";
#define MATSQRT(A) MatrixOperations::CholeskyDecomposition(A)

class MatrixOperations {
  public:
    template<typename T, int R, int C> 
    static Eigen::Matrix<T, R, C> SanitizeCovariance(const Eigen::Matrix<T, R, C>& M) {
      auto root = MATSQRT(M);
      auto square = root * root.transpose();
      return square;
    }

    template<typename T, int R>
    static Eigen::Matrix<T, R, R> CholeskyDecomposition(const Eigen::Matrix<T, R, R>& A) {
      Eigen::Matrix<T, R, R> L = Eigen::Matrix<T, R, R>::Zero();
      for(int i = 0; i < R; i++) {
        for(int j = 0; j < i + 1; j++) {
          T s = 0;
          for(int k= 0; k < j; k++) {
            s += L(i,k) * L(j,k);
          }
          // Here we check to ensure that a positive value is passed to the sqrt function.
          // This algorithm requires positive-definite matrices as inputs, but sometimes
          // particular values can fall below zero due to issues with floating point
          // arithmetic. Thus the algorithm provides a small correction where necessary.
          if(i == j) {
            auto diff = A(i,i) - s;
            if(diff < 0) {
              /*printf("Decomposition Error: i:%i,j:%i,A(%i,%i):%2.4f,s:%2.4f:\n",i,j,i,j,A(i,j),s);*/
              /*cout << A << "\n";*/
              L(i,j) = 1;
            } else L(i,j) = sqrt(diff);
          }
          else
            L(i,j) = 1.0 / L(j,j) * (A(i,j) - s);
        }
      }
      return L;
    }
    
    template<typename T, int R>
    static Eigen::Matrix<T, R, R> RankUpdate(Eigen::Matrix<T, R, R> L, Eigen::Matrix<T, R, 1> x, T sigma) {
      for(int i = 0; i < R; i++) {
        auto r = sqrt(pow(L(i,i),2) + sigma * pow(x[i],2));
        auto c = r / L(i,i);
        auto s = x[i] / L(i,i);
        L(i,i) = r;
        for(int j = i+1; j < R; j++) {
          L(i,j) = (L(i,j) + sigma * s * x[j]) / c;
          x[j] = c * x[j] - s*L(i,j);
        }
      }
      return L;
    }

    template<typename T, int R,int C>
    static Eigen::Matrix<T, R, R> HouseholderTransform(Eigen::Matrix<T, R, 2*C+R> input) {
      int rows = input.rows();
      int r = input.cols() - rows;
      T sigma;
      T a;
      T b;
      std::vector<T> v(input.cols());
      Eigen::Matrix<T, R, R> result;
      for(int k=rows-1;k>=0;k--){
        sigma=0.0;
        for(int j=0;j<=r+k;j++){
          sigma=sigma+input(k,j)*input(k,j);
        }
        a=sqrtf(sigma);
        sigma=0.0;
        for(int j=0;j<=r+k;j++){
          if(j==r+k){
            v.at(j)=(input(k,j)-a);
          }
          else{
            v.at(j)=(input(k,j));
          }
          sigma=sigma+v.at(j)*v.at(j);
        }
        a=2.0/(sigma+1e-15);
        for(int i=0;i<=k;i++){
          sigma=0.0;
          for(int j=0;j<=r+k;j++){
            sigma=sigma+input(i,j)*v.at(j);
          }
          b=a*sigma;
          for(int j=0;j<=r+k;j++){
            input(i,j)=input(i,j)-b*v.at(j);
          }
        }
      }
      for(int i=0;i<rows;i++){
        result.col(i) = input.col(r + i);
      }
      return result;
    }
};
#endif
