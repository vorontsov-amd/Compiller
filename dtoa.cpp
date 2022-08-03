#include <cmath>
#include <cstdio>

double Floor(double x) { return (x >= 0) ?  (double)(int)x : (double)(int)(x - 1); }
int isDigit(char x) { return ((x) >= '0' && (x) <= '9'); }


char* itoa(int digit, int exp, char* str)
{
    char* s = str;
    do 
    {
        *str++ = (digit % 10) + '0';
        digit = digit / 10;
    } while (digit != 0);
    if (exp > 0)
    {
        for (int i = 0, j = exp; j > i; i++, j--)
        {
            char t = s[i];
            s[i] = s[j];
            s[j] = t;
        }
    }
    return str;
}


int dtoa(double num, char* str)
{
    // ...
    if (num == 0)
    {
        //...
    }
    else
    {
        if (num < 0)
        {
            num = -num;
            *str++ = '-';
        }
        int exp = Floor(log10(num));
        if (exp > 13 || exp < -9)
        {
            num = num / pow(10, exp);
            *str++ = (int)num + '0';
            *str++ = '.';
            num = (num - int(num)) * 10;
            *str++ = (int)num + '0';
            num = (num - int(num)) * 10;
            *str++ = (int)num + '0';
            *str++ = 'e';
            if (exp < 0)
            {
                exp = -exp;
                *str++ = '-';
            }
            int e = (int)log10(exp);
            str = itoa(exp, e, str);
        }
        else
        {
            int digit = (int)num;
            num = num - digit;
            str = itoa(digit, exp, str);
            if (num != 0)
            {
                *str++ = '.';
                for (int i = 0; i < 15; i++)
                {
                    num = num * 10;
                    int digit = (int)num;
                    num = num - digit;
                    *str++ = digit + '0';
                    if (num < 1e-15 * pow(10, i)) break;
                }    
            }
        }
        *str = '\0';
    }
}




double atod(char* str)
{
    double res = 0.0;
    int sign = 1;
    if (*str == '-')
    {
        sign = -1;
        str++;
    }
    while (*str != '\0' && isDigit(*str))
    {
        res = res * 10 + *str++ - '0';
    }
    if (*str == '.')
    {
        str++;
        int exp = 0;
        while (*str != '\0' && isDigit(*str))
        {
            res = res * 10 + *str++ - '0';
            exp--;
        }
        double e = pow(10, exp);
        res *= e;
    }
    if (*str == 'e')
    {
        double w = atod(++str);
        double e = pow(10, w);
        res *= e;
    }
    res *= sign;
}




int main()
{
    char ch[32] = "";
    scanf("%s", ch);
    double x = atod(ch);
    dtoa(x, ch);
    printf("%s, %lg\n",ch, x);
}