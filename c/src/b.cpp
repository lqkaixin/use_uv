#include <stdio.h>
class Complex 
{         

private :
        double    m_real;
        double    m_imag;

public:

        //    无参数构造函数
        // 如果创建一个类你没有写任何构造函数,则系统会自动生成默认的无参构造函数，函数为空，什么都不做
        // 只要你写了一个下面的某一种构造函数，系统就不会再自动生成这样一个默认的构造函数，如果希望有一个这样的无参构造函数，则需要自己显示地写出来
        Complex(void)
        {
          printf("Complex(void)\n");
             m_real = 0.0;
             m_imag = 0.0;
        } 
        
        //    一般构造函数（也称重载构造函数）
        // 一般构造函数可以有各种参数形式,一个类可以有多个一般构造函数，前提是参数的个数或者类型不同（基于c++的重载函数原理）
        // 例如：你还可以写一个 Complex( int num)的构造函数出来
        // 创建对象时根据传入的参数不同调用不同的构造函数
        Complex(double real, double imag)
        {
          printf("Complex(double,double)\n");
             m_real = real;
             m_imag = imag;         
         }
        
        //    复制构造函数（也称为拷贝构造函数）
        //    复制构造函数参数为类对象本身的引用，用于根据一个已存在的对象复制出一个新的该类的对象，一般在函数中会将已存在对象的数据成员的值复制一份到新创建的对象中
        //    若没有显示的写复制构造函数，则系统会默认创建一个复制构造函数，但当类中有指针成员时，由系统默认创建该复制构造函数会存在风险，具体原因请查询 有关 “浅拷贝” 、“深拷贝”的文章论述
        Complex(const Complex & c)
        {
                // 将对象c中的数据成员值复制过来
          printf("Complex(const Complex&)\n");
                m_real = c.m_real;
                m_imag    = c.m_imag;
        }            
    
        // 类型转换构造函数，根据一个指定的类型的对象创建一个本类的对象
        // 例如：下面将根据一个double类型的对象创建了一个Complex对象
        Complex(double r)
        {
          printf("Complex(double)\n");
                m_real = r;
                m_imag = 0.0;
        }

        // 等号运算符重载
        // 注意，这个类似复制构造函数，将=右边的本类对象的值复制给等号左边的对象，它不属于构造函数，等号左右两边的对象必须已经被创建
        // 若没有显示的写=运算符重载，则系统也会创建一个默认的=运算符重载，只做一些基本的拷贝工作
        Complex &operator=( const Complex &rhs )
        {
          printf("Complex(==)\n");
                // 首先检测等号右边的是否就是左边的对象本，若是本对象本身,则直接返回
                if ( this == &rhs ) 
                {
                        return *this;
                }
                
                // 复制等号右边的成员到左边的对象中
                this->m_real = rhs.m_real;
                this->m_imag = rhs.m_imag;
                
               // 把等号左边的对象再次传出
               // 目的是为了支持连等 eg:    a=b=c 系统首先运行 b=c
               // 然后运行 a= ( b=c的返回值,这里应该是复制c值后的b对象)    
                return *this;
        }

};

/*
void main()
{
        // 调用了无参构造函数，数据成员初值被赋为0.0
        Complex c1，c2;

        // 调用一般构造函数，数据成员初值被赋为指定值
        Complex c3(1.0,2.5);
        // 也可以使用下面的形式
        Complex c3 = Complex(1.0,2.5);
        
        //    把c3的数据成员的值赋值给c1
        //    由于c1已经事先被创建，故此处不会调用任何构造函数
        //    只会调用 = 号运算符重载函数
        c1 = c3;
        
        //    调用类型转换构造函数
        //    系统首先调用类型转换构造函数，将5.2创建为一个本类的临时对象，然后调用等号运算符重载，将该临时对象赋值给c1
        c2 = 5.2;
        
       // 调用拷贝构造函数( 有下面两种调用方式) 
        Complex c5(c2);
        Complex c4 = c2;  // 注意和 = 运算符重载区分,这里等号左边的对象不是事先已经创建，故需要调用拷贝构造函数，参数为c2

        
        
}
*/
  Complex test1(const Complex& c)
  {
          return c;
  }
  
  Complex test2(const Complex c)
  {
         return c;
   }
   
   Complex test3()
   {
          static Complex c(1.0,5.0);
          return c;
   }
  
  Complex& test4()
  {
         static Complex c(1.0,5.0);
         return c;
  }
  
  int main()
  {
        Complex a,b;
    
        // 下面函数执行过程中各会调用几次构造函数，调用的是什么构造函数？
    
          printf("test1\n");
       test1(a);
          printf("test2\n");
       test2(a);
     
          printf("test3\n");
       b = test3();
          printf("test4\n");
       b = test4();
     
          printf("test5\n");
       test1(1.2);
       // 下面这条语句会出错吗？
          printf("test6\n");
       test2(1.2);     //test1( Complex(1.2 )) 呢？
       return 0;
  }