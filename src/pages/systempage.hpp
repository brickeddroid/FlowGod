/*generated file systempage.hpp*/
#ifndef SYSTEMPAGE_HPP
#define SYSTEMPAGE_HPP

#include "ipage.hpp"

class SystemPage : public IPage
{
    private:
    protected:
    public:
        //ctor
        SystemPage();
        //dtor
        virtual ~SystemPage();
        
        //inherited functions
	    virtual void updateContent();
		virtual void updateDvalue();
		virtual void updateValue();
};
#endif //SYSTEMPAGE_HPP
