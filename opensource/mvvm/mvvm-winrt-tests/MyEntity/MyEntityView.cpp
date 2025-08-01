#include "pch.h"
#include "MyEntityView.h"
#if __has_include("MyEntityView.g.cpp")
#include "MyEntityView.g.cpp"
#endif

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;

namespace winrt::mvvm::tests::implementation
{
    MyEntityView::MyEntityView()
        : MyEntityView(MyEntityViewModel::class_type{})
    {
    }

    MyEntityView::MyEntityView(MyEntityViewModel::class_type const& viewModel)
        : view_base(viewModel)
    {
        InitializeComponent();
    }
}
