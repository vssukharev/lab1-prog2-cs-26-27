
#pragma once

#include <iostream>
#include "tormentor.h"

std::ostream& operator<<(std::ostream& out, const Torture& tort);

/*
 Read Tormentor from 'in' in format:

 Nw Nl
 w0 l00 l01 ...
 w1 l10 l11 ...
  ...
 wi li0 li1 ... lij ... 
  ...
 wn ln0 ln1 ...

 Where 'Nw' and 'Nl' are the number of
 unique workers and labours respectively.
 The labour 'lij' from the same list is assigned 
 to worker 'wi'. In case of invalid input, returns an exception.
**/
Tormentor read_tormentor(std::istream& in);

