#pragma once

using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Data;

namespace WinRTControls
{
	[BindableAttribute]
	public ref class ImageData sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	public:
		ImageData(Platform::String^ name, Platform::String^ image, Platform::String^ category)
		{
			Name = name;
			Image = image;
			Category = category;
		}

		property Platform::String^ Name
		{
			Platform::String^ get()
			{
				return _name;
			}
			void set(Platform::String^ value)
			{
				if (_name != value)
				{
					_name = value;
					OnPropertyChanged("Name");
				}
			}
		}

		property Platform::String^ Image
		{
			Platform::String^ get()
			{
				return _image;
			}
			void set(Platform::String^ value)
			{
				if (_image != value)
				{
					_image = value;
					OnPropertyChanged("Image");
				}
			}
		}

		property Platform::String^ Category
		{
			Platform::String^ get()
			{
				return _category;
			}
			void set(Platform::String^ value)
			{
				if (_category != value)
				{
					_category = value;
					OnPropertyChanged("Category");
				}
			}
		}

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;

	private:
		Platform::String^ _name;
		Platform::String^ _image;
		Platform::String^ _category;

		void OnPropertyChanged(Platform::String^ propertyName)
		{
			//using namespace Windows::UI::Xaml::Data;
			PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
		}
	};
}


