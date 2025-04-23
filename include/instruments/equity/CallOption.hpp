#ifndef CALLOPTION_HPP
#define CALLOPTION_HPP

#include <iostream>
#include "instruments/equity/Option.hpp"

class CallOption : public Option{

};


std::ostream& operator<<(std::ostream& output_stream,const CallOption& call);

#endif 
