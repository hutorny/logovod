/*
 * Copyright (C) 2024 Eugene Hutorny <eugene@hutorny.in.ua>
 *
 * This file is a part of logovod library
 *
 * Licensed under MIT License, see full text in LICENSE
 * or visit page https://opensource.org/license/mit/
 */

#include "tests.h"

#include <chrono>
#include <deque>
#include <list>
#include <forward_list>
#include <optional>
#include <variant>
 
using namespace logovod::tests;
using namespace logovod;

TEST_F(Levels, StaticRespected) {
    L::i("Ignored");        EXPECT_EQ(message, "");
    L::d("Ignored");        EXPECT_EQ(message, "");
    L::n("Not written");    EXPECT_EQ(message, "");
    L::log(priority::notice, "ignored"); EXPECT_EQ(message, "");
    L::log(priority::warning, "logged"); EXPECT_EQ(message, "logged");
    L::w("Written");        EXPECT_EQ(message, "Written");
    L::e("Emitted");        EXPECT_EQ(message, "Emitted");
    L::a("Alerted");        EXPECT_EQ(message, "Alerted");
    L::c("Critical");       EXPECT_EQ(message, "Critical");
    L::f("Filed");          EXPECT_EQ(message, "Filed");
    EXPECT_EQ(write_count, 6);
}

TEST_F(Levels, DynamicRespected) {
    R::i("Ignored");        EXPECT_EQ(message, "");
    R::d("Ignored");        EXPECT_EQ(message, "");
    R::n("Not written");    EXPECT_EQ(message, "");
    R::w("Written");        EXPECT_EQ(message, "");
    R::log(priority::warning, "ignored"); EXPECT_EQ(message, "");
    R::log(priority::error, "logged"); EXPECT_EQ(message, "logged");
    R::e("Emitted");        EXPECT_EQ(message, "Emitted");
    R::a("Alerted");        EXPECT_EQ(message, "Alerted");
    R::c("Critical");       EXPECT_EQ(message, "Critical");
    R::f("Filed");          EXPECT_EQ(message, "Filed");
    EXPECT_EQ(write_count, 5);
}

TEST_F(Levels, RuntimeRespectedError) {
    RuntimeLevel::level(priority::error);
    R::w("Written");        EXPECT_EQ(message, "");
    R::e("Emitted");        EXPECT_EQ(message, "Emitted");
    EXPECT_EQ(write_count, 1);
}

TEST_F(Levels, RuntimeRespectedWarning) {
    RuntimeLevel::level(priority::warning);
    R::n("Not written");    EXPECT_EQ(message, "");
    R::w("Written");        EXPECT_EQ(message, "Written");
    EXPECT_EQ(write_count, 1);
}

TEST_F(Levels, RuntimeRespectedNotice) {
    RuntimeLevel::level(priority::notice);
    R::d("Ignored");        EXPECT_EQ(message, "");
    R::n("Noticed");        EXPECT_EQ(message, "Noticed");
    EXPECT_EQ(write_count, 1);     
}

TEST_F(Levels, RuntimeRespectedInformational) {
    RuntimeLevel::level(priority::informational);
    R::d("Ignored");        EXPECT_EQ(message, "");
    R::i("Informed");       EXPECT_EQ(message, "Informed");
    EXPECT_EQ(write_count, 1);
}

TEST_F(Levels, RuntimeRespectedAlert) {
    RuntimeLevel::level(priority::alert);
    R::e("Emitted");        EXPECT_EQ(message, "");
    R::a("Alerted");        EXPECT_EQ(message, "Alerted");
    EXPECT_EQ(write_count, 1);
}

TEST_F(Levels, RuntimeRespectedCritical) {    
    RuntimeLevel::level(priority::critical);
    R::e("Error");          EXPECT_EQ(message, "");
    R::log(priority::error, "ignored"); EXPECT_EQ(message, "");
    R::log(priority::critical, "logged"); EXPECT_EQ(message, "logged");
    R::c("Critical");       EXPECT_EQ(message, "Critical");
    EXPECT_EQ(write_count, 2);
}

TEST_F(Levels, RuntimeRespectedEmergency) {    
    RuntimeLevel::level(priority::emergency);
    R::e("Error");             EXPECT_EQ(message, "");
    R::c("Critical");          EXPECT_EQ(message, "");
    R::emergency("emergency"); EXPECT_EQ(message, "emergency");
    EXPECT_EQ(write_count, 1);
}
