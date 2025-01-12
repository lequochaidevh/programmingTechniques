#include <stdio.h>
int get_xor(int a, int b){
    return a^b;
}
int re_xor(int *a, int b){
    *a = *a^b;
    return *a;
}
void XorSwap(int *x, int *y) 
{
  if (x == y) return;
  *x ^= *y;
  *y ^= *x;
  *x ^= *y;
}
int main()
{
    int arr_temp[8] = {2,3,9,8,3,4,6,4};
    int top_suilt = arr_temp[0];
    int bot_suilt = arr_temp[7];
    int top_xor = arr_temp[0];
    int bot_xor = arr_temp[7];
    int resuilt = 0;
    int key_update = 0;
     printf("top_suilt = %d\n",top_suilt);
     printf("bot_suilt = %d\n",bot_suilt);
	XorSwap(&top_suilt,&bot_suilt);
    printf("top_suilt = %d\n",top_suilt);
     printf("bot_suilt = %d\n",bot_suilt);
     XorSwap(&top_suilt,&bot_suilt);
    for(int i=1; i<8; i++){
        top_xor = get_xor(top_xor,arr_temp[i]);
    }
    for(int i=6; i>=0; i--){
        bot_xor = get_xor(bot_xor,arr_temp[i]);
    }
    printf("top_xor = %d\n",top_xor);
    printf("bot_xor = %d\n",bot_xor);
    
    //top_xor = top_xor^bot_suilt;
    //bot_xor = bot_xor^top_suilt;
    resuilt = top_xor^bot_xor;
    printf("%d\n",resuilt);

    top_xor = top_xor^resuilt;
    bot_xor = bot_xor^2;
    resuilt = top_xor^bot_xor^(5*resuilt)^resuilt;
    printf("%d\n",resuilt);



}
