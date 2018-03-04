#include "pch.h"
#include "ImagesRepository.h"

using namespace WinRTControls;

ImagesRepository::ImagesRepository()
{
	_images = ref new Vector<ImageData^>({
		ref new ImageData("Image 1", "ms-appx:///Assets/im_1.jpg", "Cat 1"),
		ref new ImageData("Image 2", "ms-appx:///Assets/im_2.jpg", "Cat 2"),
		ref new ImageData("Image 3", "ms-appx:///Assets/im_3.jpg", "Cat 3"),
		ref new ImageData("Image 4", "ms-appx:///Assets/im_4.jpg", "Cat 1"),
		ref new ImageData("Image 5", "ms-appx:///Assets/im_5.jpg", "Cat 2"),
		ref new ImageData("Image 6", "ms-appx:///Assets/im_6.jpg", "Cat 3"),
		ref new ImageData("Image 7", "ms-appx:///Assets/im_7.jpg", "Cat 1"),
		ref new ImageData("Image 8", "ms-appx:///Assets/im_8.jpg", "Cat 2"),
		ref new ImageData("Image 9", "ms-appx:///Assets/im_9.jpg", "Cat 3"),
		ref new ImageData("Image 10", "ms-appx:///Assets/im_10.jpg", "Cat 1"),
		ref new ImageData("Image 11", "ms-appx:///Assets/im_11.jpg", "Cat 2"),
		ref new ImageData("Image 12", "ms-appx:///Assets/im_12.jpg", "Cat 3"),
		ref new ImageData("Image 13", "ms-appx:///Assets/im_13.jpg", "Cat 1"),
		ref new ImageData("Image 14", "ms-appx:///Assets/im_14.jpg", "Cat 2"),
		ref new ImageData("Image 15", "ms-appx:///Assets/im_15.jpg", "Cat 3"),
		ref new ImageData("Image 16", "ms-appx:///Assets/im_16.jpg", "Cat 1"),
		ref new ImageData("Image 17", "ms-appx:///Assets/im_17.jpg", "Cat 2"),
		ref new ImageData("Image 18", "ms-appx:///Assets/im_18.jpg", "Cat 3"),
		ref new ImageData("Image 19", "ms-appx:///Assets/im_19.jpg", "Cat 1"),
		ref new ImageData("Image 20", "ms-appx:///Assets/im_20.jpg", "Cat 2"),
		ref new ImageData("Image 21", "ms-appx:///Assets/im_21.jpg", "Cat 3"),
		ref new ImageData("Image 22", "ms-appx:///Assets/im_22.jpg", "Cat 1"),
		ref new ImageData("Image 23", "ms-appx:///Assets/im_23.jpg", "Cat 2"),
		ref new ImageData("Image 24", "ms-appx:///Assets/im_24.jpg", "Cat 3"),
		ref new ImageData("Image 25", "ms-appx:///Assets/im_25.jpg", "Cat 1"),
		ref new ImageData("Image 26", "ms-appx:///Assets/im_26.jpg", "Cat 2"),
		ref new ImageData("Image 27", "ms-appx:///Assets/im_27.jpg", "Cat 3")
	});
}

Windows::Foundation::Collections::IVector<ImageData^>^ ImagesRepository::GetImagesData()
{
	if (this->_images == nullptr)
	{
		this->_images = ref new Platform::Collections::Vector<ImageData^>();
	}
	return this->_images;
}
