#include <fstream>
#include <queue>
#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "animator.h"

const std::string CL_RESET   = "\033[0m";
const std::string BG_RED     = "\033[1;41m";
const std::string BG_YELLOW  = "\033[1;43m";
const std::string BG_GREEN   = "\033[1;42m";
const std::string BG_CYAN    = "\033[1;46m";
const std::string BG_BLUE    = "\033[1;44m";
const std::string BG_MAGENTA = "\033[1;45m";
const std::string FG_WHITE   = "\033[1;37m";

TEST(AnimatorTest, StepForwardOnce)
{
    // Load the animation and capture output
    Animator animator;
    animator.load("../samples/animation_1");
    std::stringstream ss;
    animator.set_stream(ss);

    // Open the frame buffer
    std::ifstream fin;
    fin.open("../samples/animation_1/00.txt", std::ifstream::in);

    // Step forward one frame
    animator.forward(1);

    // Make line-by-line comparisons of frames
    std::string expected_line;
    std::string actual_line;
    while (getline(fin, expected_line))
    {
        getline(ss, actual_line);
        ASSERT_STREQ(expected_line.c_str(), actual_line.c_str());
    }
    // Close the frame buffer
    fin.close();
}

TEST(AnimatorTest, StepForwardAndThrough)
{
    // Load the animation and capture output
    Animator animator;
    animator.load("../samples/animation_1");
    std::stringstream ss;
    animator.set_stream(ss);

    // For each frame in the full animation
    for (int i = 0; i < 24; i++)
    {
        // Step forward one frame
        animator.forward(1);

        // Use index to derive filepath to next frame
        std::string index = std::to_string(i);
        if (i < 10)
        {
            index = "0" + index;
        }
        std::string file_path = "../samples/animation_1/" + index + ".txt";

        // Open the frame buffer
        std::ifstream fin;
        fin.open(file_path, std::ifstream::in);

        // Make line-by-line comparisons of frames
        std::string expected_line;
        std::string actual_line;
        while (getline(fin, expected_line))
        {
            getline(ss, actual_line);
            ASSERT_STREQ(expected_line.c_str(), actual_line.c_str());
        }
        // Close the frame buffer
        fin.close();
    }
}

TEST(AnimatorTest, StepForwadAndThroughColors)
{
    // Define color cycle
    std::queue<std::string> color_cycle;
    color_cycle.push(BG_RED);
    color_cycle.push(BG_YELLOW);
    color_cycle.push(BG_GREEN);
    color_cycle.push(BG_CYAN);
    color_cycle.push(BG_BLUE);
    color_cycle.push(BG_MAGENTA);

    // Load the animation and capture output
    Animator animator;
    animator.load("../samples/animation_1");
    std::stringstream ss;
    animator.set_stream(ss);

    // For each frame in the full animation
    for (int i = 0; i < 24; i++)
    {
        // Step forward one frame
        animator.forward(1);

        // Use index to derive filepath to next frame
        std::string index = std::to_string(i);
        if (i < 10)
        {
            index = "0" + index;
        }
        std::string file_path = "../samples/animation_1/" + index + ".txt";

        // Open the frame buffer
        std::ifstream fin;
        fin.open(file_path, std::ifstream::in);

        // Make line-by-line comparisons of frames
        std::string expected_line;
        std::string actual_line;
        std::string bg_color;
        while (getline(fin, expected_line))
        {
            // Get next color code from cycle
            bg_color = color_cycle.front();
            color_cycle.pop();
            color_cycle.push(bg_color);

            // Apply coloring to expected line
            expected_line = FG_WHITE + bg_color + expected_line + CL_RESET;

            getline(ss, actual_line);
            ASSERT_STREQ(expected_line.c_str(), actual_line.c_str());
        }
        // Close the frame buffer
        fin.close();
    }
}
