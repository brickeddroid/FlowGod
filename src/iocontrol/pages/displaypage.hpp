/*generated file displaypage.hpp*/
#ifndef DISPLAYPAGE_HPP
#define DISPLAYPAGE_HPP

#include "ipage.hpp"

class DisplayPage : public IPage
{
    private:
    protected:
    public:
        //ctor
        DisplayPage();
        //dtor
        virtual ~DisplayPage();
        
        //inherited functions
	    virtual void updateContent();
		virtual void updateDvalue();
		virtual void updateValue();
};
#endif //DISPLAYPAGE_HPP
