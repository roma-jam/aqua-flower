/*
 * application.h
 *
 *  Created on: 19 ����. 2014 �.
 *      Author: r.leonov
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_


#include <kl_lib_f100.h>
#include <string.h>

class App_t {
public:
    Thread *PThd;
    void Init();
};

extern App_t App;

#endif /* APPLICATION_H_ */
