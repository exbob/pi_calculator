#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// 使用莱布尼茨级数计算π: π/4 = 1 - 1/3 + 1/5 - 1/7 + 1/9 - ...
double calculate_pi_leibniz(long long iterations) {
  double pi = 0.0;
  int sign = 1;
  
  for (long long i = 0; i < iterations; i++) {
      pi += sign * (1.0 / (2 * i + 1));
      sign *= -1;
  }
  
  return pi * 4;
}

// 使用蒙特卡洛方法计算π
double calculate_pi_monte_carlo(long long iterations) {
  long long inside_circle = 0;
  
  // 设置随机种子
  srand((unsigned int)time(NULL));
  
  for (long long i = 0; i < iterations; i++) {
      double x = (double)rand() / RAND_MAX * 2.0 - 1.0;  // -1 到 1
      double y = (double)rand() / RAND_MAX * 2.0 - 1.0;  // -1 到 1
      
      if (x * x + y * y <= 1.0) {
          inside_circle++;
      }
  }
  
  return 4.0 * inside_circle / iterations;
}

// 使用马青公式计算π (更快收敛)
double calculate_pi_machin(int precision) {
  long long iterations = (long long)pow(10, precision + 2);
  double pi = 0.0;
  
  // π/4 = 4*arctan(1/5) - arctan(1/239)
  // 使用泰勒级数展开arctan
  
  // 计算 arctan(1/5)
  double arctan_1_5 = 0.0;
  double term = 1.0 / 5.0;
  for (long long i = 0; i < iterations && fabs(term) > 1e-15; i++) {
      if (i % 2 == 0) {
          arctan_1_5 += term / (2 * i + 1);
      } else {
          arctan_1_5 -= term / (2 * i + 1);
      }
      term *= (1.0 / 25.0);  // (1/5)^2
  }
  
  // 计算 arctan(1/239)
  double arctan_1_239 = 0.0;
  term = 1.0 / 239.0;
  for (long long i = 0; i < iterations && fabs(term) > 1e-15; i++) {
      if (i % 2 == 0) {
          arctan_1_239 += term / (2 * i + 1);
      } else {
          arctan_1_239 -= term / (2 * i + 1);
      }
      term *= (1.0 / (239.0 * 239.0));  // (1/239)^2
  }
  
  pi = 4 * (4 * arctan_1_5 - arctan_1_239);
  return pi;
}

void print_usage() {
  printf("用法: ./pi_calculator <方法> <精度>\n");
  printf("方法:\n");
  printf("  1 - 莱布尼茨级数 (较慢但简单)\n");
  printf("  2 - 蒙特卡洛方法 (随机采样)\n");
  printf("  3 - 马青公式 (快速收敛)\n");
  printf("精度: 小数点后位数 (1-15)\n");
  printf("示例: ./pi_calculator 3 10\n");
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
      print_usage();
      return 1;
  }
  
  int method = atoi(argv[1]);
  int precision = atoi(argv[2]);
  
  if (method < 1 || method > 3) {
      printf("错误: 方法必须是 1, 2, 或 3\n");
      print_usage();
      return 1;
  }
  
  if (precision < 1 || precision > 15) {
      printf("错误: 精度必须在 1-15 之间\n");
      return 1;
  }
  
  printf("开始计算π值...\n");
  printf("方法: ");
  
  clock_t start_time = clock();
  double pi_calculated = 0.0;
  long long iterations = 0;
  
  switch (method) {
      case 1:
          printf("莱布尼茨级数\n");
          iterations = (long long)pow(10, precision + 6);
          pi_calculated = calculate_pi_leibniz(iterations);
          break;
      case 2:
          printf("蒙特卡洛方法\n");
          iterations = (long long)pow(10, precision + 6);
          pi_calculated = calculate_pi_monte_carlo(iterations);
          break;
      case 3:
          printf("马青公式\n");
          pi_calculated = calculate_pi_machin(precision);
          iterations = (long long)pow(10, precision + 2);
          break;
  }
  
  clock_t end_time = clock();
  double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
  
  // 输出结果
  printf("\n=== 计算结果 ===\n");
  printf("计算得到的π值: %.*f\n", precision, pi_calculated);
  printf("标准π值:       %.15f\n", M_PI);
  printf("误差:           %.2e\n", fabs(pi_calculated - M_PI));
  printf("迭代次数:       %lld\n", iterations);
  printf("计算时间:       %.6f 秒\n", cpu_time);
  printf("计算速度:       %.2e 次/秒\n", iterations / cpu_time);
  
  // CPU性能评估
  printf("\n=== CPU性能评估 ===\n");
  double performance_score = iterations / cpu_time / 1e6;  // 百万次操作/秒
  printf("性能得分:       %.2f MOPS (百万次操作/秒)\n", performance_score);
  
  if (performance_score > 100) {
      printf("CPU性能:        优秀\n");
  } else if (performance_score > 50) {
      printf("CPU性能:        良好\n");
  } else if (performance_score > 20) {
      printf("CPU性能:        一般\n");
  } else {
      printf("CPU性能:        较低\n");
  }
  
  return 0;
}
