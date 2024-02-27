//-----------------------------------------------------------------------------
//
// Exeptions vs ret codes benchmark header
//
//-----------------------------------------------------------------------------

#pragma once

constexpr int NCALL = 10000;
constexpr int NBMK = 1;

int call_exc(int x);
int call_outer_exc(int x);
int call_inner_exc(int x);

int call_retc(int x) noexcept;
int call_outer_retc(int x) noexcept;
int call_inner_retc(int x) noexcept;

int startup_exc();
int startup_retc() noexcept;
