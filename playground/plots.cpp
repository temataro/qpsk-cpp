#include <cmath>
#include <matplot/matplot.h>
// #include <vector>
// #include <complex>

#define PI 3.1415926536
//
// using namespace matplot;
// int main() {
//   double symT = 1 / 9600;
//   double alpha = 0.3333;
//   std::vector<double> t = linspace(-10, 10, 100);
//   // sinc
//   std::vector<double> rrcFilt(100);
//   // lambda function syntax:
//   //    [=] (inputDataType &input) -> outputDataType {function here;}
//   auto elementwise_sinc = [=](double &x) -> double {
//     return std::sin(PI * x) / (PI * x);
//   };
//   
//   // transform(input_start, inp_end, out_start, lambda_function)
//   std::transform(t.begin(), t.end(), rrcFilt.begin(), elementwise_sinc);
//   
//   plot(t, rrcFilt);
//   show();
//
//   return 0;
// }
//

int main() {
    using namespace matplot;

    std::vector<double> x = linspace(0, 2 * pi);
    std::vector<double> y1 = transform(x, [](auto x) { return sin(x); });
    std::vector<double> y2 = transform(x, [](auto x) { return sin(x - 0.25); });
    std::vector<double> y3 = transform(x, [](auto x) { return sin(x - 0.5); });
    plot(x, y1, x, y2, "--", x, y3, ":");

    show();
    return 0;
}
