#pragma once

#include <collection.h>

using namespace Platform::Collections;

namespace WinRTControls
{
	public ref class ImagesRepository sealed
	{
	public:
		ImagesRepository();

		Windows::Foundation::Collections::IVector<ImageData^>^ GetImagesData();

	private:
		Windows::Foundation::Collections::IVector<ImageData^>^ _images;
	};
}
