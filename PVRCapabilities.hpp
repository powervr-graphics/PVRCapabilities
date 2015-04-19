#pragma once

#include <string>
#include <unordered_map>

namespace pvr {
enum class Family {
	SERIES6,
	SERIES6XE,
	SERIES6XT,
	SERIES7XE,
	SERIES7XT,
	UNKNOWN
};
enum EFlags {
	CAP_FP16       = 1 << 1,        // USC has 16-bit float units
	CAP_RAYTRACING = 1 << 2,        // Accelerated ray tracing
};
struct SCapabilities {
	Family family_;                 // PowerVR variant
	unsigned int fp16_ops_;         // Peak operations per-clock (FP16)
	unsigned int fp32_ops_;         // Peak operations per-clock (FP32)
	unsigned int bilinear_samples_; // Peak bilinear samples per-clock
	unsigned int flags_;            // Capability flags

	SCapabilities()
	:
    family_(Family::UNKNOWN),
	fp16_ops_(0),
	fp32_ops_(0),
	bilinear_samples_(0),
	flags_(0)
	{}

	SCapabilities(
		Family family,
		unsigned int fp16_ops,
		unsigned int fp32_ops,
		unsigned int bilinear_samples,
		unsigned int flags
	):
		family_(family),
		fp16_ops_(fp16_ops),
		fp32_ops_(fp32_ops),
		bilinear_samples_(bilinear_samples),
		flags_(flags)
		{}
};
	/*!****************************************************************************
	 * @function        DetermineGPUCapabilities
	 * @brief           Returns a struct of peak per-clock performance stats & 
     *                  hardware capability flags for a given GL_RENDERER string
	 *                  Note: "Han" and "Hood" cases handle legacy GL_RENDERER string formatting
	 *
	 * @param[in]       renderer_string   GL_RENDERER string value
	 * @param[out]      capabilities      Struct containing GPU capabilities
	 * @return          bool              True if the model number is recognized
	 ******************************************************************************/
	static bool DetermineGPUCapabilities(const std::string& renderer_string, SCapabilities& capabilities) {
        capabilities = {};

		static const std::unordered_map<std::string, SCapabilities> gpu_models {
			// { "GPU model", { Family, Ops(FP16), Ops(FP32), Bilinear samples, Flags}},
            // PowerVR Series6
			{ "G6200",  { Family::SERIES6,   128,  128,  4,  0        } },
            { "Han",    { Family::SERIES6,   128,  128,  4,  0        } }, // May be G6200 or G6230
			{ "G6230",  { Family::SERIES6,   192,  128,  4,  CAP_FP16 } },
			{ "G6400",  { Family::SERIES6,   256,  256,  8,  0        } },
			{ "Hood",   { Family::SERIES6,   256,  256,  8,  0        } }, // May be G6400 or G6430
			{ "G6430",  { Family::SERIES6,   384,  256,  8,  CAP_FP16 } },
			{ "G6630",  { Family::SERIES6,   576,  384,  12, CAP_FP16 } },
            // PowerVR Series6XE
			{ "G6020",  { Family::SERIES6XE, 32,   8,    1,  CAP_FP16 } },
			{ "G6050",  { Family::SERIES6XE, 48,   32,   2,  CAP_FP16 } },
			{ "G6060",  { Family::SERIES6XE, 48,   32,   2,  CAP_FP16 } },
			{ "G6100",  { Family::SERIES6XE, 96,   64,   4,  CAP_FP16 } },
			{ "G6110",  { Family::SERIES6XE, 96,   64,   4,  CAP_FP16 } },
            // PowerVR Series6XT
			{ "GX6240", { Family::SERIES6XT, 256,  128,  4,  CAP_FP16 } },
			{ "GX6250", { Family::SERIES6XT, 256,  128,  4,  CAP_FP16 } },
			{ "GX6450", { Family::SERIES6XT, 512,  256,  8,  CAP_FP16 } },
			{ "GX6650", { Family::SERIES6XT, 768,  384,  12, CAP_FP16 } },
            // PowerVR Series7XE
			{ "GE7400", { Family::SERIES7XE, 48,   32,   2,  CAP_FP16 } },
			{ "GE7800", { Family::SERIES7XE, 96,   64,   4,  CAP_FP16 } },
            // PowerVR Series7XT
			{ "GT7200", { Family::SERIES7XT, 256,  128,  4,  CAP_FP16 } },
			{ "GT7400", { Family::SERIES7XT, 512,  256,  8,  CAP_FP16 } },
			{ "GT7600", { Family::SERIES7XT, 768,  384,  12, CAP_FP16 } },
			{ "GT7800", { Family::SERIES7XT, 1024, 512,  16, CAP_FP16 } },
			{ "GT7900", { Family::SERIES7XT, 2048, 1024, 32, CAP_FP16 } },
			// PowerVR Wizard
			{ "GR6500", { Family::SERIES6XT, 512,  256,  8,  CAP_FP16 | CAP_RAYTRACING } }
		};

		for (const auto &model : gpu_models) {
			if (renderer_string.find(model.first) != std::string::npos) {
				capabilities = model.second;
				return true;
			}
		}

		return false;
	}
}
