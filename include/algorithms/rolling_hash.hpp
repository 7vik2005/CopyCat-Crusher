#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <cstddef>
class RollingHash{
public:
    RollingHash(
        const std::vector<std::string>& tokens,
        std::size_t windowSize
    );

    void compute();

    [[nodiscard]]
    const std::vector<std::uint64_t>& getHashes() const;

private:
    const std::vector<std::string>& tokens;

    std::size_t windowSize;

    std::vector<std::uint64_t> hashes;

    static constexpr std::uint64_t base=257ULL;

    static constexpr std::uint64_t modulus=
        1000000007ULL;

    [[nodiscard]]
    std::uint64_t tokenValue(
        const std::string& token
    ) const;
};
