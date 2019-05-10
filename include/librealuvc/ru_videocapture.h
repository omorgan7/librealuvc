// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2019 Leap Motion Corporation. All Rights Reserved.

#ifndef LIBREALUVC_RU_VIDEOCAPTURE_H
#define LIBREALUVC_RU_VIDEOCAPTURE_H 1

// We implement a librealuvc::VideoCapture which extends the
// cv::VideoCapture, allowing code using cv::VideoCapture to
// be ported with minimal changes.

#include "ru_common.h"
#include "ru_uvc.h"
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

namespace librealuvc {

using std::shared_ptr;
using std::unique_ptr;

// Frame data is allocated by lower-level code (e.g. Windows Media Foundation),
// so cv::Mat's referring to lower-level buffers will have a custom
// cv::MatAllocator to release the buffer when it's no longer being used
// by any cv::Mat.

// librealuvc::VideoCapture overrides all methods of cv::VideoCapture,
// but it may represent either a librealuvc device or an opencv device.
// When it's an opencv device, it will forward method calls to get
// the normal cv::VideoCapture behavior.

class IVideoStream {
 public:
  virtual ~IVideoStream() { }
};

// A librealuvc::VideoCapture may refer to a device-specific
// IPropertyDriver which may intercept the get/set calls.

enum HandlerResult {
  kHandlerFalse   = 0,
  kHandlerTrue    = 1,
  kHandlerNotDone = 2
};

class IPropertyDriver {
 public:
  virtual ~IPropertyDriver() { }
  
  virtual int get_frame_fixup() { return 0; }
  
  virtual HandlerResult get_prop_range(int prop_id, double* min, double* max) = 0;
  
  virtual HandlerResult get_prop(int prop_id, double* val) = 0;
  
  virtual HandlerResult set_prop(int prop_id, double val) = 0;
};

typedef std::function<
  shared_ptr<IPropertyDriver>(const shared_ptr<uvc_device>&)
> PropertyDriverMaker;

LIBREALUVC_EXPORT void register_property_driver(
  uint16_t vendor_id,
  uint16_t product_id,
  PropertyDriverMaker maker
);

// Leap-specific device controls taking care not to match
enum CapPropLeap {
  CAP_PROP_LEAP_BASE = 100,
  CAP_PROP_LEAP_HDR  = 101,
  CAP_PROP_LEAP_LEDS = 102,
};

class LIBREALUVC_EXPORT VideoCapture : public cv::VideoCapture {
 protected:
  bool is_opencv_;
  unique_ptr<cv::VideoCapture> opencv_;
  bool is_realuvc_;
  int vendor_id_;
  int product_id_;
  shared_ptr<librealuvc::uvc_device> realuvc_;
  shared_ptr<IPropertyDriver> driver_;
  shared_ptr<IVideoStream> istream_;
  cv::Mat reusable_image_;
  
 public:
  VideoCapture();
  VideoCapture(int index);
  VideoCapture(const cv::String& filename);
  VideoCapture(const cv::String& filename, int api_reference);
  
  virtual ~VideoCapture();
  
  virtual double get(int prop_id) const;
  virtual bool grab();
  virtual bool isOpened() const;
  
  virtual bool open(int index);
  virtual bool open(const cv::String& filename);
  virtual bool open(const cv::String& filename, int api_preference);
  
  virtual VideoCapture& operator>>(cv::Mat& image);
  virtual VideoCapture& operator>>(cv::UMat& image);
  
  virtual bool read(cv::OutputArray image);
  virtual void release();
  virtual bool retrieve(cv::OutputArray image, int flag = 0);
  virtual bool set(int prop_id, double value);

  // Does it support extended functionality through librealuvc ?
  virtual bool is_extended() const;
  
  virtual int get_vendor_id() const;
  virtual int get_product_id() const;
  
  virtual bool get_prop_range(int prop_id, double* min_val, double* max_val);
  
  virtual bool get_xu(int ctrl, void* data, int len);
  virtual bool set_xu(int ctrl, const void* data, int len);
  
  inline cv::Mat& get_reusable_image() { return reusable_image_; }
};
  
} // end librealuvc


#endif