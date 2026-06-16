#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <cstddef>
struct Fingerprint{
    std::uint64_t hash=0;
    std::size_t position=0;
};

class Winnowing{
public:
    Winnowing(
        const std::vector<std::uint64_t>& hashes,
        std::size_t windowSize
    );

    void compute();

    [[nodiscard]]
    const std::vector<Fingerprint>&
    getFingerprints() const;

private:
    const std::vector<std::uint64_t>& hashes;

    std::size_t windowSize;

    std::vector<Fingerprint> fingerprints;
};
