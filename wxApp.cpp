//
// Created by mmccanney on 4/10/23.
//
#include "wxApp.h"
#include "wxMainFrame.h"

bool MyApp::OnInit()
{

    //MyFrame *frame = new MyFrame();
    MyFrame *frame = new MyFrame(NULL, -1, "Time Tracker", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE);
    frame->Show(true);

    if (argc > 0) {
        wxString fn {argv[1]};
        //frame->ReadMprjFile(fn);
    }

    return true;
}
