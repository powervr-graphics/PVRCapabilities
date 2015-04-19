#pragma once

#include <string>
#include <unordered_map>

namespace pvr {
enum ERogueGeneration {
	PVR_SERIES6,
	PVR_SERIES6XE,
	PVR_SERIES6XT,
	PVR_SERIES7XE,
	PVR_SERIES7XT,
	PVR_SERIES_UNKNOWN
};
enum ECapabilityFlags {
	PVR_CAP_FP16            = 1 << 1,        // USC has 16-bit float units
	PVR_CAP_RAYTRACING      = 1 << 2,        // Accelerated ray tracing
};
struct SCapabilities {
	ERogueGeneration generation;             // Rogue variant
	unsigned int fp16_operations_per_clock;  // Peak operations per-clock (FP16)
	unsigned int fp32_operations_per_clock;  // Peak operations per-clock (FP32)
	unsigned int bilinear_samples_per_clock; // Peak bilinear samples per-clock
	unsigned int flags;                      // Capability flags

	SCapabilities()
	:
	generation(PVR_SERIES_UNKNOWN),
	fp16_operations_per_clock(0),
	fp32_operations_per_clock(0),
	bilinear_samples_per_clock(0),
	flags(0)
	{}

	SCapabilities(
		ERogueGeneration g,
		unsigned int fp16,
		unsigned int fp32,
		unsigned int bs,
		unsigned int f
	):
		generation(g),
		fp16_operations_per_clock(fp16),
		fp32_operations_per_clock(fp32),
		bilinear_samples_per_clock(bs),
		flags(f)
		{}
};
	/*!****************************************************************************
	 * @function        DetermineRogueCapabilities
	 * @brief           Conservative estimate of a PowerVR Rogue GPU's capabilities.
	 *                  "Han" and "Hood" cases handle legacy GL_RENDERER string formatting
	 *
	 * @param[in]       renderer_string   GL_RENDERER string value
	 * @param[out]      capabilities      Struct containing GPU capabilities
	 * @return          bool              True if the model number is recognized
	 ******************************************************************************/
	bool DetermineRogueCapabilities(const std::string& renderer_string, SCapabilities& capabilities) {
		if (std::string::npos == renderer_string.find("Rogue")) {
			return false;
		}

		capabilities = {};

		static const std::unordered_map<std::string, SCapabilities> caps {
			// PowerVR Series6
			{ "G6200",  { PVR_SERIES6, 128, 128, 4,  0                } },
			{ "Han",    { PVR_SERIES6, 128, 128, 4,  0                } },
			{ "G6230",  { PVR_SERIES6, 192, 128, 4,  PVR_CAP_FP16     } },
			{ "G6400",  { PVR_SERIES6, 256, 256, 8,  0                } },
			{ "Hood",   { PVR_SERIES6, 256, 256, 8,  0                } },
			{ "G6430",  { PVR_SERIES6, 384, 256, 8,  PVR_CAP_FP16     } },
			{ "G6630",  { PVR_SERIES6, 576, 384, 12, PVR_CAP_FP16     } },
			// PowerVR  Series6XE
			{ "G6020",  { PVR_SERIES6XE, 32, 8,  1,  PVR_CAP_FP16     } },
			{ "G6050",  { PVR_SERIES6XE, 48, 32, 2,  PVR_CAP_FP16     } },
			{ "G6060",  { PVR_SERIES6XE, 48, 32, 2,  PVR_CAP_FP16     } },
			{ "G6100",  { PVR_SERIES6XE, 96, 64, 4,  PVR_CAP_FP16     } },
			{ "G6110",  { PVR_SERIES6XE, 96, 64, 4,  PVR_CAP_FP16     } },
			// PowerVR Series6XT
			{ "GX6240", { PVR_SERIES6XT, 256, 128, 4,  PVR_CAP_FP16   } },
			{ "GX6250", { PVR_SERIES6XT, 256, 128, 4,  PVR_CAP_FP16   } },
			{ "GX6450", { PVR_SERIES6XT, 512, 256, 8,  PVR_CAP_FP16   } },
			{ "GX6650", { PVR_SERIES6XT, 768, 384, 12, PVR_CAP_FP16   } },
			// PowerVR Series7XE
			{ "GE7400", { PVR_SERIES7XE, 48,  32,  2,  PVR_CAP_FP16   } },
			{ "GE7800", { PVR_SERIES7XE, 96,  64,  4,  PVR_CAP_FP16   } },
			// PowerVR Series7XT
			{ "GT7200", { PVR_SERIES7XT, 256,  128,  4,  PVR_CAP_FP16 } },
			{ "GT7400", { PVR_SERIES7XT, 512,  256,  8,  PVR_CAP_FP16 } },
			{ "GT7600", { PVR_SERIES7XT, 768,  384,  12, PVR_CAP_FP16 } },
			{ "GT7800", { PVR_SERIES7XT, 1024, 512,  16, PVR_CAP_FP16 } },
			{ "GT7900", { PVR_SERIES7XT, 2048, 1024, 32, PVR_CAP_FP16 } },
			// PowerVR Wizard
			{ "GR6500", { PVR_SERIES6XT, 512, 256, 8, PVR_CAP_FP16 | PVR_CAP_RAYTRACING } }
		};

		for (const auto &kv : caps) {
			if (renderer_string.find(kv.first) != std::string::npos) {
				capabilities = kv.second;
				return true;
			}
		}

		return false;
	}
}
