#ifndef VISUAL_PROCESSOR_H
#define VISUAL_PROCESSOR_H

#include <memory>

#include <aslam/common/macros.h>

namespace cv { class Mat; }

namespace aslam {

class VisualFrame;
class Camera;

/// \class VisualPipeline
/// \brief An interface for processors that turn images into VisualFrames
///
/// This is the abstract interface for visual processors that turn raw images
/// into VisualFrame data. The underlying pipeline may include undistortion
/// or rectification, image contrast enhancement, feature detection and
/// descriptor computation, or other operations.
///
/// The class has two Camera calibration structs that represent the
/// extrinsic calibration of the camera.
/// The "input" calibration (getInputCamera()) represents the calibration of
/// raw camera, before any image processing, resizing, or undistortion
/// has taken place. The "output" calibration (getOutputCamera())
/// represents the calibration parameters of the images and keypoints that get
/// set in the VisualFrame struct. These are the camera parameters after
/// image processing, resizing, undistortion, etc.
class VisualPipeline {
public:
  ASLAM_POINTER_TYPEDEFS(VisualPipeline);
  ASLAM_DISALLOW_EVIL_CONSTRUCTORS(VisualPipeline);

  VisualPipeline(){}

  /// \brief Construct a visual pipeline from the input and output cameras
  VisualPipeline(const std::shared_ptr<Camera>& input_camera,
                 const std::shared_ptr<Camera>& output_camera);

  virtual ~VisualPipeline();

  /// \brief Add an image to the visual processor.
  ///
  /// This function is called by a user when an image is received.
  /// The processor then processes the images and constructs a VisualFrame.
  /// This method constructs a basic frame and passes it on to processFrame().
  ///
  /// \param[in] image          The image data.
  /// \param[in] system_stamp   The host time in integer nanoseconds since epoch.
  /// \param[in] hardware_stamp The camera's hardware timestamp. Can be set to "invalid".
  /// \returns                  The visual frame built from the image data.
  std::shared_ptr<VisualFrame> processImage(const cv::Mat& image,
                                            int64_t system_stamp,
                                            int64_t hardware_stamp) const;

  /// \brief Get the input camera that corresponds to the image
  ///        passed in to processImage().
  ///
  /// Because this processor may do things like image undistortion or
  /// rectification, the input and output camera may not be the same.
  virtual const std::shared_ptr<Camera>& getInputCamera() const { return input_camera_; }

  /// \brief Get the output camera that corresponds to the VisualFrame
  ///        data that comes out.
  ///
  /// Because this pipeline may do things like image undistortion or
  /// rectification, the input and output camera may not be the same.
  virtual const std::shared_ptr<Camera>& getOutputCamera() const { return output_camera_; }

  /// \brief Process the frame and fill the results into the frame variable.
  ///
  /// This function can be used to chain together pipelines that do different things.
  /// The top level function will already fill in the timestamps and the output camera.
  /// \param[in]     image The image data.
  /// \param[in/out] frame The visual frame. This will be constructed before calling.
  virtual void processFrame(const cv::Mat& image,
                            std::shared_ptr<VisualFrame>* frame) const = 0;

protected:
  /// \brief The intrinsics of the raw image.
  std::shared_ptr<Camera> input_camera_;
  /// \brief The intrinsics of the raw image.
  std::shared_ptr<Camera> output_camera_;
};
}  // namespace aslam

#endif // VISUAL_PROCESSOR_H
