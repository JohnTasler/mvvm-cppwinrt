//*********************************************************
//
//    Copyright (c) Microsoft. All rights reserved.
//    This code is licensed under the MIT License.
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
//    ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
//    TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//    PARTICULAR PURPOSE AND NONINFRINGEMENT.
//
//*********************************************************
#pragma once

#include "pch.h"
#include <winrt/mvvm.tests.h>
#include "module.g.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace winrt::mvvm::tests
{
    TEST_CLASS(mvvmwinrttests)
    {
    public:
        TEST_METHOD(TestMethod1)
        {
            MyEntityViewModel myEntityViewModel;
            MyEntityView myEntityView{ myEntityViewModel };
        }
    };
}
