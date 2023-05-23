#include "iostream"

#include "Eigen/Core"
#include "Eigen/SVD"
#include "Eigen/Geometry"

#include "opencv2/core.hpp"
#include "opencv2/core/eigen.hpp"
#include "opencv2/imgcodecs.hpp"

#define GRAYSCALE_MAX 255
#define LENNA_PATH "../lenna.png"
#define IMG1 "../res1.png"
#define IMG2 "../res2.png"
#define IMG3 "../res3.png"

using namespace std;
class Op {
  Eigen::Vector4f v{1, 1.5, 2, 3};
  Eigen::Vector4f w{0, 1, 2, 4};
  Eigen::Matrix4f i{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
  Eigen::Matrix4f j{{4, 3, 2, 1}, {8, 7, 6, 5}, {12, 11, 10, 9}, {16, 15, 14, 13}};

 public:
  void res1() {
    cout << v << endl;
    cout << w << endl;
    cout << v + w << endl;
    cout << v.dot(w) << endl;
  }

  void res2() {
    cout << i << endl;
    cout << j << endl;
    cout << i + j << endl;
    cout << i * j << endl;
    cout << i * v << endl;
  }
};

using namespace cv;
class opencv {
  std::string image_path;
  Mat img;
  Eigen::MatrixXd matrix;

 public:
  explicit opencv(const std::string &path) {
    image_path = samples::findFile(path + LENNA_PATH);
    img = imread(image_path, IMREAD_GRAYSCALE);
    cv2eigen(img, matrix);
    matrix /= GRAYSCALE_MAX;
  }

  void res(const std::string &path) {
    Eigen::BDCSVD<Eigen::MatrixXd> bdcsvd(matrix, Eigen::ComputeFullU | Eigen::ComputeFullV);
    const auto &U = bdcsvd.matrixU();
    const auto &singularValues = bdcsvd.singularValues();
    const auto &V = bdcsvd.matrixV();

    Mat resImg1, resImg2, resImg3;

    Eigen::MatrixXd S1(512, 1);
    for (auto i = 0; i < 512; ++i) {
      S1(i, 0) = 0;
    }
    S1(0, 0) = singularValues(0);

    Eigen::MatrixXd res1 = U * S1 * V.leftCols(1).transpose() * GRAYSCALE_MAX;
    eigen2cv(res1, resImg1);
    imwrite(path + IMG1, resImg1);

    Eigen::MatrixXd S2(512, 10);
    for (auto i = 0; i < 512; ++i) {
      for (auto j = 0; j < 10; ++j) {
        if (i == j) {
          S2(i, j) = singularValues(i);
        } else {
          S2(i, j) = 0;
        }
      }
    }
    Eigen::MatrixXd res2 = U * S2 * V.leftCols(10).transpose() * GRAYSCALE_MAX;
    eigen2cv(res2, resImg2);
    imwrite(path + IMG2, resImg2);

    Eigen::MatrixXd S3(512, 50);
    for (auto i = 0; i < 512; ++i) {
      for (auto j = 0; j < 50; ++j) {
        if (i == j) {
          S3(i, j) = singularValues(i);
        } else {
          S3(i, j) = 0;
        }
      }
    }
    Eigen::MatrixXd res3 = U * S3 * V.leftCols(50).transpose() * GRAYSCALE_MAX;
    eigen2cv(res3, resImg3);
    imwrite(path + IMG3, resImg3);
  }
};

using namespace Eigen;
class trans {
  Vector3d p1{1, 2, 3};
  Vector3d p2{4, 5, 6};
 public:
  void res() {
    Vector3d src = p1 - p2;
    Matrix3d t;
    t = AngleAxisd(0.25 * M_PI, Vector3d::UnitX())
        * AngleAxisd(double(1) / 6 * M_PI, Vector3d::UnitY())
        * AngleAxisd(double(1) / 3 * M_PI, Vector3d::UnitZ());
    Vector3d dst = t * src;
    std::cout << dst + p2 << std::endl;
  }
};

int main([[maybe_unused]] int argc, char* argv[]) {

  const std::string str(argv[0]);
  auto p = str.rbegin();
  for (; *p != '/'; p++) {
  }
  const std::string path(str.begin(), p.base());

  Op res{};
  res.res1();
  res.res2();

  opencv oc(path);
  oc.res(path);

  trans trans{};
  trans.res();

  return 0;
}
