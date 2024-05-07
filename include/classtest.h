/******************************************************************************
 * Project: stm32 - E4                                                        *
 * -----                                                                      *
 * Copyright 2024 - James Clarke                                              *
 * -----                                                                      *
 * Licence:                                                                   *
 *     Use of this source code is governed by an MIT-style                    *
 *     license that can be found in the LICENSE file or at                    *
 *     https://opensource.org/licenses/MIT.                                   *
 ******************************************************************************/
#ifndef CLASSTEST_H
#define CLASSTEST_H

#ifdef __cplusplus
extern "C" {
#endif

class TestCPP;
extern TestCPP TestObj;

class TestCPP
{
public:
    TestCPP() : Counter(0) {}
    TestCPP(uint8_t Value) :
	Counter(Value){}

    void Increment(void)
    {
        Counter++;
    }

    uint8_t Get(void) 
    {
        return Counter;
    }

private:
    uint8_t Counter;
};

#ifdef __cplusplus
}
#endif

#endif // CLASSTEST_H