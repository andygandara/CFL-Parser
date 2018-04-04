//
//  Andres Gandara
//  inputbuf.h
//
//  Created by Andy Gandara on 3/30/18.
//  Copyright © 2018 Andy Gandara. All rights reserved.
//

#ifndef __INPUT_BUFFER__H__
#define __INPUT_BUFFER__H__

#include <string>
#include <vector>

class InputBuffer {
public:
    void GetChar(char&);
    char UngetChar(char);
    std::string UngetString(std::string);
    bool EndOfInput();
    
private:
    std::vector<char> input_buffer;
};

#endif  //__INPUT_BUFFER__H__
