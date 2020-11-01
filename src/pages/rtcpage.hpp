/*generated file rtcpage.hpp*/
#ifndef RTCPAGE_HPP
#define RTCPAGE_HPP

#include "ipage.hpp"

class RtcPage : public IPage
{
    private:
    protected:
    public:
        //ctor
        RtcPage();
        //dtor
        virtual ~RtcPage();
        
        //inherited functions
	    virtual void updateContent();
		virtual void updateDvalue();
		virtual void updateValue();
};
#endif //RTCPAGE_HPP
