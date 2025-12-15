/**
 * @file animation.hpp
 * @brief Animation system for smooth transitions in DSAV
 *
 * Provides easing functions, animation structures, and an animation controller
 * for managing sequential and parallel animations.
 */

#pragma once

#include <functional>
#include <queue>
#include <vector>
#include <glm/glm.hpp>

namespace dsav {

/**
 * @brief Easing functions for smooth animations
 *
 * All easing functions take a normalized time value t ∈ [0, 1]
 * and return a transformed value, also typically in [0, 1].
 */
namespace easing {
    /** Linear interpolation (no easing) */
    float linear(float t);

    /** Ease in and out (smooth start and end) */
    float easeInOut(float t);

    /** Ease in (slow start, fast end) */
    float easeIn(float t);

    /** Ease out (fast start, slow end) */
    float easeOut(float t);

    /** Bounce effect at the end */
    float easeOutBounce(float t);

    /** Elastic/spring effect at the end */
    float easeOutElastic(float t);

    /** Back easing (overshoots and returns) */
    float easeOutBack(float t);
}

/**
 * @brief Single animation instance
 *
 * An animation runs for a specified duration and calls an update function
 * with a normalized time value (0 to 1) on each frame.
 */
struct Animation {
    float duration;                              // Total duration in seconds
    float elapsed = 0.0f;                        // Time elapsed so far
    std::function<void(float t)> updateFn;       // Called each frame with t ∈ [0, 1]
    std::function<void()> onComplete;            // Called when animation completes
    std::function<float(float)> easingFn = easing::easeInOut;  // Easing function

    /** Check if animation has completed */
    bool isComplete() const { return elapsed >= duration; }

    /** Update the animation with delta time */
    void update(float deltaTime);
};

/**
 * @brief Animation controller for managing animation queues
 *
 * Supports sequential animations (queue) and parallel animation groups.
 * Animations can be paused, their speed can be adjusted, and they can be cleared.
 */
class AnimationController {
public:
    /** Add animation to the end of the queue */
    void enqueue(Animation anim);

    /** Add multiple animations to run in parallel as a group */
    void enqueueParallel(std::vector<Animation> anims);

    /** Update all active animations with delta time */
    void update(float deltaTime);

    /** Check if currently paused */
    bool isPaused() const { return m_paused; }

    /** Set paused state */
    void setPaused(bool paused) { m_paused = paused; }

    /** Get speed multiplier */
    float getSpeedMultiplier() const { return m_speed; }

    /** Set speed multiplier (0.1x to 5.0x typical range) */
    void setSpeedMultiplier(float speed) { m_speed = speed; }

    /** Clear all pending animations */
    void clear();

    /** Check if there are any animations (queued or running) */
    bool hasAnimations() const;

    /** Check if currently processing a parallel group */
    bool isProcessingParallelGroup() const { return !m_parallelGroup.empty(); }

private:
    std::queue<Animation> m_queue;                // Sequential animation queue
    std::vector<Animation> m_parallelGroup;       // Current parallel animation group
    bool m_paused = false;                        // Paused state
    float m_speed = 1.0f;                         // Speed multiplier
};

/**
 * @brief Helper functions to create common animations
 */

/** Create animation that moves a position to a target */
Animation createMoveAnimation(glm::vec2& pos, const glm::vec2& target, float duration);

/** Create animation that changes a color to a target */
Animation createColorAnimation(glm::vec4& color, const glm::vec4& target, float duration);

/** Create animation that scales a value to a target */
Animation createScaleAnimation(float& scale, float target, float duration);

/** Create animation that fades alpha to a target */
Animation createFadeAnimation(float& alpha, float target, float duration);

/** Create a delay/pause animation */
Animation createDelayAnimation(float duration);

} // namespace dsav
