#include<StdAfx.h>
# include <iostream>
# include <windows.h>
# include <time.h>
// գրադարանը հարկավոր է  Sleep() ֆունկցիան օգտագործելու համար
# include <conio.h>
// գրադարանը հարկավոր է  kbhit()  և getch() ֆունկցիաները օգտագործելու համար
using namespace std;char main_color[] = "color ##";
// գույնի համար
int snake_size, change_x, change_y, coordinates_x[1000], coordinates_y[1000], food_x = -1, food_y = -1;
/*
    snake_size – օձի չափը
    change_x, change_y – որ կողմ է շարժվում օձը
    coordinates_x[1000], coordinates_y[1000] -  զանգվածներ, որոնք պահում են օձի մարմնի մասերի կորդինատները
    food_x, food_y – օձի կերակուրի կորդինատները
    օձի գլխի կորդինատները պահվում են ՝ coordinates_x[1], coordinates_y[1]
*/
char symbol, a[1000][1000];
/*
    symbol – պահում է իրենում  ASCII կոդը  սեղմված ստեղնի
    a[1000][1000] –  աղյուսակն է, որում կայանում է ամբողջ խաղը
*/
const int N = 13, M = 17, INTERVAL = 200;
/*
    հաստատուներ,
    N – աղյուսակի չափը , իսկ ավելի կոնկրետ բարձրությունը
    M – աղյուսակի լայնություն
    INTERVAL – ինտերվալը միլիվայրկյաններով, որի ընթացքում օձը պետք է շարժվի
*/
void change_color()
// ֆունկցիա, որը փոփոխում է կոնսուլի գույները
{
     char x = rand() % 16, y = rand() % 16;
     if (x == y)
      ++y;
     y %= 16;
     if (x > 9)
      x += 'A' - 10;
     else
      x += '0';
     if (y > 9)
      y += 'A' - 10;
     else
      y += '0';
     // ընտրում ենք պատահական գույն
     main_color[6] = x;
     main_color[7] = y;
     system(main_color);
     // փոխում ենք գույնը
}
void change_direction()
// ֆունկցիա, որը հաշվի է առնում սեղմված ստեղնը
{
     symbol = getch();
     // հաշվի ենք առնում սեղմված ստեղնը getch() ֆունկցիայի օգնությամբ
     switch (symbol)
     {
         case 'w': if(change_x != 1 || change_y != 0) { change_x = -1; change_y = 0; } break;
         case 'a': if(change_x != 0 || change_y != 1) { change_x = 0; change_y = -1; } break;
         case 's': if(change_x != -1 || change_y != 0) { change_x = 1; change_y = 0; } break;
         case 'd': if(change_x != 0 || change_y != -1) { change_x = 0; change_y = 1; } break;
         // օձի կառավարման համար օգտագործվում է w a s d ստեղները 
         case 32 : change_color(); break;
         // եթե սեղմենք բացատ, կփոխվի կոնսուլի գույները
         default : break; 
     }
}
void show_table()
// ֆունկցիա, որը դուրս է բերում աղյուսակը
{
    system("cls");
    // մաքրում ենք  կոնսուլը
    for (int i = 0; i <= N + 1; ++i)
     for (int j = 0; j <= M + 1; ++j)
      cout << (i == 0 || j == 0 || i == N + 1 || j == M + 1 ? '#' : a[i][j]) << (j <= M ? "" : "\n");
    // դուրս ենք բերում աղյուսակը և եզրերը
}
void clear_snake_on_table()
// մաքրում ենք կորդինատները, որտեղով անցել է օձը
{
     for (int i = 1; i <= snake_size; ++i)
      a[coordinates_x[i]][coordinates_y[i]] = ' ';
}
void show_snake_on_table()
// ներկում ենք  օձի կորդինատները
{
     if(change_x == 1 && change_y == 0) a[coordinates_x[1]][coordinates_y[1]] = 'v';
     if(change_x == -1 && change_y == 0) a[coordinates_x[1]][coordinates_y[1]] = '^';
     if(change_x == 0 && change_y == 1) a[coordinates_x[1]][coordinates_y[1]] = '>';
     if(change_x == 0 && change_y == -1) a[coordinates_x[1]][coordinates_y[1]] = '<';
     // փոխում ենք գլխի դիրքը 
     for (int i = 2; i <= snake_size; ++i)
      a[coordinates_x[i]][coordinates_y[i]] = '@';
     // ներկում ենք օձին
}
bool game_over()
// ստուգում ենք օձը` ինքը իրեն կերել է
{
     for (int i = 2; i <= snake_size; ++i)
      if (coordinates_x[1] == coordinates_x[i] && coordinates_y[1] == coordinates_y[i]) return true;
     // եթե օձի գլխի կորդինատները հավասար են օձի մարմնի կորդինատներից  որևէ
//մեկին, ապա օձը ինքը իրեն կերել է 
     return false;
     // եթե կորդինատները տարբեր են, ապա խաղը շարունակվում է
}
void check_coordinates()
//  ստուգում ենք օձը դուրս է եկել իր դաշտից թե ոչ, եթե դուրս է եկել, ապա վերադարձնում ենք դաշտ
{
    if (coordinates_x[1] > N) coordinates_x[1] = 1;
    if (coordinates_x[1] < 1) coordinates_x[1] = N;
    if (coordinates_y[1] > M) coordinates_y[1] = 1;
    if (coordinates_y[1] < 1) coordinates_y[1] = M;
}
void next_step()
//  հաջորդ քայլի ֆունկցիան որով օձը շարժվում է 1 բջիջով
{
     clear_snake_on_table();
     // մաքրում ենք աղյուսակը օձից 
     for (int i = snake_size; i >= 2; --i)
     {
         coordinates_x[i] = coordinates_x[i - 1];
         coordinates_y[i] = coordinates_y[i - 1];
     }
     // շարժում ենք օձի մարմինը 
     coordinates_x[1] += change_x;
     coordinates_y[1] += change_y;
     // շարժում ենք օձի գլուխը 
     check_coordinates();
     // ստուգում ենք կորդինատները նորմալ են 
     if(coordinates_x[1] == food_x && coordinates_y[1] == food_y)
     {
         snake_size++;
         food_x = -1;
         food_y = -1;
     }
     // եթե օձի գլուխը այնտեղ է որտեղ և կերը, ապա մեծացնում ենք օձի չափերը և մաքրում ենք օձի կորդինատները 
     show_snake_on_table();
     // նկարում ենք օձը
 
     if (game_over() == true)
     // եթե օձը կծել է ինքը իրեն
     {
         cout << "You're looser! \n";
         // հայտնում ենք խաղացողի պարտվելու  մասին
         system("pause");
         // կանգնեցնում ենք խաղը
         exit(0);
         // ավարտում ենք ծրագիրը
     }
}
bool food_check()
// ֆունկցիա, որը ստուգում է ավելորդ կերի առկայությունը
{
     if(food_x == -1 && food_y == -1) return false;
     // եթե կերակուրի կորդինատները որոշված չեն վերադարձնում է true
     return true;
     // հակառակ դեպքում false
}
void place_food()
// ֆունկցիա, որը ավելացնում է կերակուրը դաշտում
{
     srand(time(NULL));
     for (int i = 1; i <= 9; ++i)
     {
         int x = rand(), y = rand();
         if(x < 0) x *= -1;
         if(y < 0) y *= -1;
         x %= (N + 1);
         y %= (M + 1);
         if(x == 0) ++x;
         if(y == 0) ++y;
         if(a[x][y] != '@' && a[x][y] != 'v' && a[x][y] != '^' && a[x][y] != '<' && a[x][y] != '>')
         {
             food_x = x;
             food_y = y;
             a[x][y] = '+';
             return;
         }
     }
     // ընտրում ենք պատահական տեղ կերի համար
}
void standart_settings()
// նախնական կարգավորումը
{
     snake_size = 2;
     // օձի չափը - 2
     coordinates_x[1] = 1;
     coordinates_y[1] = 2;
     coordinates_x[2] = 1;
     coordinates_y[2] = 1;
     // օձը զբաղեցնում է  {1,1} կորդինատից 2 վանդակ դեպի աջ 
     change_x = 0;
     change_y = 1;
     // օձը շարժվում է աջ
}
int main ()
{
    standart_settings();
    // տալիս ենք ստանդարտ կարգավորումները 
    while (1)
    // անվերջ ցիկլ
    {
        if (kbhit() == true)
        // եթե սեղմված տեղնը
         change_direction();
         // աշխատեցնում ենք սեղմված ստեղնը 
        next_step();
        // շարժում ենք օձին 
        if(food_check() == false)
         place_food();
        // եթե չկա կերակուր ապա դնում ենք 
        show_table();     // նկարում ենք օձ
    Sleep(INTERVAL); // քնեցնում ենք ծրագիրը տրված ինտերվալում
}}
  
