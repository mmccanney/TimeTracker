//
// Created by mmccanney on 4/10/23.
//

#ifndef __WXWIDGETS_WXAPP_H__
#define __WXWIDGETS_WXAPP_H__

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
//#include <wx/cmdline.h>

enum
{
    ID_Hello = 1
};

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};


#endif //__WXWIDGETS_WXAPP_H__
