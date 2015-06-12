

int ff(int a, int b)
{
    //getCallStack();
    *(int*)0 = 4;
    return a+b;
}

int gg(int a, int b)
{
    return ff(0x12000000,0x34000000);
}

int hh(int a, int b)
{
    return gg(0x56000000, 0x78000000);
}

int crash_test()
{
    hh(0xab000000,0xcd000000);
    // CallStack stack;
    return 0;
}