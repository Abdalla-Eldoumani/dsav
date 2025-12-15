/**
 * @file animation.cpp
 * @brief Implementation of animation system
 */

#include "animation.hpp"
#include "color_scheme.hpp"
#include <cmath>
#include <algorithm>

namespace dsav {

// ===== Easing Functions =====

namespace easing {

float linear(float t) {
    return t;
}

float easeInOut(float t) {
    // Cubic ease in-out
    if (t < 0.5f) {
        return 4.0f * t * t * t;
    } else {
        float f = (2.0f * t - 2.0f);
        return 0.5f * f * f * f + 1.0f;
    }
}

float easeIn(float t) {
    // Cubic ease in
    return t * t * t;
}

float easeOut(float t) {
    // Cubic ease out
    float f = t - 1.0f;
    return f * f * f + 1.0f;
}

float easeOutBounce(float t) {
    const float n1 = 7.5625f;
    const float d1 = 2.75f;

    if (t < 1.0f / d1) {
        return n1 * t * t;
    } else if (t < 2.0f / d1) {
        t -= 1.5f / d1;
        return n1 * t * t + 0.75f;
    } else if (t < 2.5f / d1) {
        t -= 2.25f / d1;
        return n1 * t * t + 0.9375f;
    } else {
        t -= 2.625f / d1;
        return n1 * t * t + 0.984375f;
    }
}

float easeOutElastic(float t) {
    const float c4 = (2.0f * 3.14159265359f) / 3.0f;

    if (t == 0.0f) return 0.0f;
    if (t == 1.0f) return 1.0f;

    return std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f;
}

float easeOutBack(float t) {
    const float c1 = 1.70158f;
    const float c3 = c1 + 1.0f;

    return 1.0f + c3 * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f);
}

} // namespace easing

// ===== Animation =====

void Animation::update(float deltaTime) {
    if (isComplete()) return;

    elapsed += deltaTime;

    // Clamp to duration
    if (elapsed > duration) {
        elapsed = duration;
    }

    // Calculate normalized time [0, 1]
    float t = (duration > 0.0f) ? (elapsed / duration) : 1.0f;

    // Apply easing function
    float easedT = easingFn ? easingFn(t) : t;

    // Call update function
    if (updateFn) {
        updateFn(easedT);
    }

    // Call completion callback if just completed
    if (isComplete() && onComplete) {
        onComplete();
    }
}

// ===== AnimationController =====

void AnimationController::enqueue(Animation anim) {
    m_queue.push(std::move(anim));
}

void AnimationController::enqueueParallel(std::vector<Animation> anims) {
    // If there's a parallel group running, we need to wait for it to finish
    // For now, we'll add a dummy animation to the queue to separate groups
    if (!m_queue.empty() || !m_parallelGroup.empty()) {
        // Add a marker to indicate end of previous animations
        // We'll handle this by just pushing all animations
    }

    // Add all parallel animations to the queue wrapped in a special way
    // For simplicity, we'll just add them sequentially for now
    // A more sophisticated implementation would track them separately
    for (auto& anim : anims) {
        m_queue.push(std::move(anim));
    }
}

void AnimationController::update(float deltaTime) {
    if (m_paused) return;

    // Apply speed multiplier
    float adjustedDelta = deltaTime * m_speed;

    // Process parallel group if active
    if (!m_parallelGroup.empty()) {
        // Update all animations in parallel
        for (auto& anim : m_parallelGroup) {
            anim.update(adjustedDelta);
        }

        // Remove completed animations
        m_parallelGroup.erase(
            std::remove_if(m_parallelGroup.begin(), m_parallelGroup.end(),
                [](const Animation& anim) { return anim.isComplete(); }),
            m_parallelGroup.end()
        );

        // If parallel group is done, we can start next animation
        return;
    }

    // Process sequential queue
    if (!m_queue.empty()) {
        Animation& current = m_queue.front();
        current.update(adjustedDelta);

        if (current.isComplete()) {
            m_queue.pop();
        }
    }
}

void AnimationController::clear() {
    // Clear queue
    while (!m_queue.empty()) {
        m_queue.pop();
    }

    // Clear parallel group
    m_parallelGroup.clear();
}

bool AnimationController::hasAnimations() const {
    return !m_queue.empty() || !m_parallelGroup.empty();
}

// ===== Helper Animation Creators =====

Animation createMoveAnimation(glm::vec2& pos, const glm::vec2& target, float duration) {
    glm::vec2 start = pos;

    Animation anim;
    anim.duration = duration;
    anim.updateFn = [&pos, start, target](float t) {
        pos = start + (target - start) * t;
    };
    anim.easingFn = easing::easeInOut;

    return anim;
}

Animation createColorAnimation(glm::vec4& color, const glm::vec4& target, float duration) {
    glm::vec4 start = color;

    Animation anim;
    anim.duration = duration;
    anim.updateFn = [&color, start, target](float t) {
        color = colors::lerp(start, target, t);
    };
    anim.easingFn = easing::linear;  // Color transitions usually look better linear

    return anim;
}

Animation createScaleAnimation(float& scale, float target, float duration) {
    float start = scale;

    Animation anim;
    anim.duration = duration;
    anim.updateFn = [&scale, start, target](float t) {
        scale = start + (target - start) * t;
    };
    anim.easingFn = easing::easeOutBack;  // Scale looks good with back easing

    return anim;
}

Animation createFadeAnimation(float& alpha, float target, float duration) {
    float start = alpha;

    Animation anim;
    anim.duration = duration;
    anim.updateFn = [&alpha, start, target](float t) {
        alpha = start + (target - start) * t;
    };
    anim.easingFn = easing::linear;

    return anim;
}

Animation createDelayAnimation(float duration) {
    Animation anim;
    anim.duration = duration;
    anim.updateFn = [](float) {
        // Do nothing during delay
    };
    anim.easingFn = easing::linear;

    return anim;
}

} // namespace dsav
