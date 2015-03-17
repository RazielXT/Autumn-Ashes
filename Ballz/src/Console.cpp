#include "stdafx.h"
#include "Console.h"

template<> OgreConsole *Singleton<OgreConsole>::msSingleton=0;

#define CONSOLE_LINE_LENGTH 85
#define CONSOLE_LINE_COUNT 15
OgreConsole::OgreConsole()
{
    start_line=0;
}

OgreConsole::~OgreConsole()
{

}

void OgreConsole::init(Ogre::SceneManager *mgr)
{


    scene=mgr;

    lastPrompt="_";
    prompt="_";

    height=0;
    visible=false;


    textbox=OverlayManager::getSingleton().createOverlayElement("TextArea","ConsoleText");
    textbox->setCaption(">_");
    textbox->setMetricsMode(GMM_RELATIVE);
    textbox->setPosition(0,0);
    textbox->setParameter("font_name","Console");
    textbox->setParameter("colour_top","1 1 1");
    textbox->setParameter("colour_bottom","1 1 1");
    textbox->setParameter("char_height","0.03");

    overlay=OverlayManager::getSingleton().create("Console");
    overlay->add2D((OverlayContainer*)textbox);
    overlay->show();

}

void OgreConsole::shutdown()
{

    initialized=false;
    overlay->remove2D((OverlayContainer*)textbox);

    // delete node;
    // delete textbox;
    // delete overlay;
}

void OgreConsole::onKeyPressed(const OIS::KeyEvent &arg)
{


    if (arg.key == OIS::KC_TAB)
    {
        setVisible(!visible);
    }

    if(height<1)
        return;
    if (arg.key == OIS::KC_RETURN && prompt!="_")
    {

        lastPrompt=prompt;
        prompt=prompt.substr(0,prompt.length()-1);
        print(prompt);

        //split the parameter list
        const char *str=prompt.c_str();
        std::vector<String> params;
        String param="";
        for(unsigned int c=0; c<prompt.length(); c++)
        {
            if(str[c]==' ')
            {
                if(param.length())
                    params.push_back(param);
                param="";
            }
            else
                param+=str[c];
        }
        if(param.length())
            params.push_back(param);

        //try to execute the command
        std::map<String,void(*)(std::vector<String>&)>::iterator i;
        for(i=commands.begin(); i!=commands.end(); i++)
        {
            if((*i).first==params[0])
            {
                if((*i).second)
                    (*i).second(params);
                break;
            }
        }

        prompt="_";
    }
    else if (arg.key == OIS::KC_BACK)
    {
        prompt=prompt.substr(0,max<int>(0,prompt.length()-2));
        prompt+="_";
    }
    else if (arg.key == OIS::KC_UP)
        prompt=lastPrompt;
    else if (arg.key == OIS::KC_PGUP)
    {
        if(start_line>10)
            start_line-=10;
        else
            start_line=0;
    }
    else if (arg.key == OIS::KC_PGDOWN)
    {
        if(lines.size()<CONSOLE_LINE_COUNT)
            start_line=0;
        else if(lines.size()-start_line-CONSOLE_LINE_COUNT>10)
            start_line+=10;
        else
            start_line+=lines.size()-start_line-CONSOLE_LINE_COUNT;
    }
    else
    {
        char legalchars[]="abcdefghijklmnopqrstuvwxyz1234567890 ";
        for(int c=0; c<37; c++)
        {
            if(legalchars[c]==arg.text)
            {
                prompt=prompt.substr(0,prompt.length()-1);
                prompt+=arg.text;
                prompt+="_";
                break;
            }
        }
    }
    update_overlay=true;
}
bool OgreConsole::frameStarted(const Ogre::FrameEvent &evt)
{

    if(visible&&height<1)
    {
        height+=evt.timeSinceLastFrame*2;
        //textbox->show();
        if(height>=1)
        {
            height=1;
        }
    }
    else if(!visible&&height>0)
    {
        height-=evt.timeSinceLastFrame*2;
        if(height<=0)
        {
            height=0;
            overlay->hide();
        }
    }

    textbox->setPosition(0,(height-1)*0.5f);

    if(update_overlay)
    {
        String text;
        std::list<String>::iterator i,start,end;

        //make sure is in range
        if(start_line>lines.size())
            start_line=lines.size();

        int lcount=0;
        start=lines.begin();
        for(size_t c=0; c<start_line; c++)
            start++;
        end=start;
        for(int c=0; c<CONSOLE_LINE_COUNT; c++)
        {
            if(end==lines.end())
                break;
            end++;
        }
        for(i=start; i!=end; i++)
            text+=(*i)+"\n";

        //add the prompt
        text+="> "+prompt;

        textbox->setCaption(text);
        update_overlay=false;
    }

    return true;
}

void OgreConsole::print(const String &text)
{
    //subdivide it into lines
    const char *str=text.c_str();
    int start=0,count=0;
    int len=text.length();
    String line;
    for(int c=0; c<len; c++)
    {
        if(str[c]=='\n'||line.length()>=CONSOLE_LINE_LENGTH)
        {
            lines.push_back(line);
            line="";
        }
        if(str[c]!='\n')
            line+=str[c];
    }
    if(line.length())
        lines.push_back(line);
    if(lines.size()>CONSOLE_LINE_COUNT)
        start_line=lines.size()-CONSOLE_LINE_COUNT;
    else
        start_line=0;
    update_overlay=true;
}

void OgreConsole::setVisible(bool visible)
{
    this->visible=visible;
    if(visible)
    {
        overlay->show();
    }
}

void OgreConsole::addCommand(const String &command, void (*func)(std::vector<String>&))
{
    commands[command]=func;
}

void OgreConsole::removeCommand(const String &command)
{
    commands.erase(commands.find(command));
}