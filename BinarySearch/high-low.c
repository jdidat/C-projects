#include <stdio.h>
int main() {
  int high = 100;
  int low = 1;
  int guess;
  char ans;
  int flag = 0;
  printf("Welcome to the High Low game...\n");
  while (flag == 0) {
    printf("Think of a number between 1 and 100 and press <enter>");
    getchar();
    do {
      guess = (high + low)/2;
      printf("Is it higher than %d? (y/n)\n", guess);
      scanf("%c", &ans);
      getchar();
      switch(ans){
        case 'y' : low = guess + 1; break;
        case 'n' : high = guess - 1; break;
        default : printf("Type y or n\n"); break;
      }
    } while (low <= high);
    if (ans == 'y') {
      guess++;
    }
    printf("\n>>>>> the number is %d\n\n", guess);
    printf("Do you want to continue playing? (y/n)\n");
    scanf("%c", &ans);
    getchar();
    if (ans == 'n') {
      flag++;
    }
    high = 100;
    low = 1;
  }
  printf("Thanks for playing!!!\n");
  return 0;
}
