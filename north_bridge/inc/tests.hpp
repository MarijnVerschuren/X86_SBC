//
// Created by marijn on 11/17/25.
//
#ifndef TESTS_HPP
#define TESTS_HPP
#include "main.hpp"


// TODO: constraint test time domain


typedef void(*test_fn)(DUT_t* top, uint32_t c);



void test_RAM_access(DUT_t* top, uint32_t c);
void test_IO_access(DUT_t* top, uint32_t c);


#endif //TESTS_HPP
