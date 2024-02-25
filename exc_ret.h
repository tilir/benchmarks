#pragma once

constexpr int NCALL = 1000;
constexpr int NBMK = 1;

int call_exc(int x);
int call_outer_exc(int x);
int call_inner_exc(int x);

int call_retc(int x);
int call_outer_retc(int x);
int call_inner_retc(int x);
