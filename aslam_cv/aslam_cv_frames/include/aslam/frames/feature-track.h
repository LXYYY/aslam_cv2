#ifndef ASLAM_FEATURE_TRACK_H_
#define ASLAM_FEATURE_TRACK_H_

#include <memory>
#include <vector>

#include <aslam/frames/keypoint-identifier.h>

namespace aslam {

class FeatureTrack {
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  ASLAM_POINTER_TYPEDEFS(FeatureTrack);

  FeatureTrack() = delete;

  explicit FeatureTrack(size_t track_id) : track_id_(track_id) {}

  FeatureTrack(size_t track_id, size_t num_reserve_keypoints) : FeatureTrack(track_id) {
    keypoint_identifiers_.reserve(num_reserve_keypoints);
  }

  inline double getDiagonalSqOfDisparityBoundingBox() const {
    double u_min = std::numeric_limits<double>::max();
    double u_max = std::numeric_limits<double>::min();
    double v_min = std::numeric_limits<double>::max();
    double v_max = std::numeric_limits<double>::min();

    for (const KeypointIdentifier& kid : getKeypointIdentifiers()) {
      const Eigen::Block<Eigen::Matrix2Xd, 2, 1> keypoint = kid.getKeypointMeasurement();
      u_min = std::min(u_min, keypoint(0));
      u_max = std::max(u_max, keypoint(0));
      v_min = std::min(v_min, keypoint(1));
      v_max = std::max(v_max, keypoint(1));
    }
    CHECK_GE(u_max, u_min);
    CHECK_GE(v_max, v_min);
    return (Eigen::Vector2d(u_min, v_min) - Eigen::Vector2d(u_max, v_max)).squaredNorm();
  }

  inline size_t getTrackId() const {
    return track_id_;
  }

  inline const KeypointIdentifierList& getKeypointIdentifiers() const {
    return keypoint_identifiers_;
  }

  inline size_t getTrackLength() const {
    return keypoint_identifiers_.size();
  }

  inline const KeypointIdentifier& getFirstKeypointIdentifier() const {
    CHECK(hasObservations()) << "Feature track is empty!";
    return keypoint_identifiers_.front();
  }

  inline const KeypointIdentifier& getLastKeypointIdentifier() const {
    CHECK(hasObservations()) << "Feature track is empty!";
    return keypoint_identifiers_.back();
  }

  inline void popLastKeypointIdentifier() {
    keypoint_identifiers_.pop_back();
  }

  inline void addKeypointObservationAtBack(const std::shared_ptr<const aslam::VisualNFrame> nframe,
                                           const size_t frame_idx, const size_t keypoint_index) {
    keypoint_identifiers_.push_back(KeypointIdentifier::create(nframe, frame_idx, keypoint_index));
  }

  inline bool hasObservations() const {
    return !keypoint_identifiers_.empty();
  }

 private:
  /// Track id.
  size_t track_id_;
  /// Keypoints on the track.
  KeypointIdentifierList keypoint_identifiers_;
};
typedef Aligned<std::vector, FeatureTrack>::type FeatureTracks;
}  // namespace aslam
#include "aslam/frames/feature-track-inl.h"
#endif  // ASLAM_FEATURE_TRACK_H_
