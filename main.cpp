// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "wxApp.h"

wxIMPLEMENT_APP(MyApp);



//#include <iostream>
//#include "task.h"
//
//int main() {
//
//    tsk::Task t1;       // can't do tsk::Task t1();   - it thinks you're defining a function!!!
//    //tsk::Task t2();       // can't do tsk::Task t1();   - it thinks you're defining a function!!!
//    tsk::Task t3{};     // can't do tsk::Task t1();   - it thinks you're defining a function!!!
//    tsk::Task t4(5);    // this however is ok. refer to Meyers discussion about braced initialization
//    tsk::Task t5{5};    // this however is ok. refer to Meyers discussion about braced initialization
//    tsk::Task t6;
//
//    std::cout << "task 3 id = " << t3.id() << std::endl;
//    std::cout << "task 4 id = " << t4.id() << std::endl;
//    std::cout << "task 5 id = " << t5.id() << std::endl;
//    std::cout << "task 6 id = " << t6.id() << std::endl;
//    std::cout << "task 1 id = " << t1.id() << std::endl;
//
//    return 0;
//}
