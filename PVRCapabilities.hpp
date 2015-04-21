#pragma once

#include <string>
#include <unordered_map>

namespace pvr {
enum class Family {
	SERIES5,
	SERIES5XT,
	SERIES6,
	SERIES6XE,
	SERIES6XT,
	SERIES7XE,
	SERIES7XT,
	UNKNOWN
};
enum class ShaderEngine {
	USSE1,
	USSE2,
	USC,
	UNKNOWN
};
enum EFlags {
	CAP_FP16         = 1 << 1,      // USC has 16-bit float units
	CAP_RAYTRACING   = 1 << 2,      // Ray tracing units 
	CAP_TESSELLATION = 1 << 3       // Tessellation units 
};
struct SCapabilities {
	Family family_;                 // PowerVR variant
	ShaderEngine shader_engine_;    // Shader engine variant
	unsigned int fp16_ops_;         // Peak operations per-clock (FP16)
	unsigned int fp32_ops_;         // Peak operations per-clock (FP32)
	unsigned int bilinear_samples_; // Peak bilinear samples per-clock
	unsigned int flags_;            // Capability flags

	SCapabilities()
	:
	family_(Family::UNKNOWN),
	shader_engine_(ShaderEngine::UNKNOWN),
	fp16_ops_(0),
	fp32_ops_(0),
	bilinear_samples_(0),
	flags_(0)
	{}

	SCapabilities(
	Family family,
	ShaderEngine shader_engine,
	unsigned int fp16_ops,
	unsigned int fp32_ops,
	unsigned int bilinear_samples,
	unsigned int flags
	):
	family_(family),
	shader_engine_(shader_engine),
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
	 *                  Note: Series5XT GL_RENDERER string doesn't report the number of GPU cores.
	 *                  Peak performance scales linearly, so performance can be calculated as
	 *                  'stat * num_gpu_cores'
	 *                  Note: "Han" and "Hood" cases handle legacy GL_RENDERER string formatting
	 *
	 * @param[in]       renderer_string   GL_RENDERER string value
	 * @param[out]      capabilities      Struct containing GPU capabilities
	 * @return          bool              True if the model number is recognized
	 ******************************************************************************/
	static bool DetermineGPUCapabilities(const std::string& renderer_string, SCapabilities& capabilities) {
        capabilities = {};

		static const std::unordered_map<std::string, SCapabilities> gpu_models {
			// { "GPU model", { Family, ShaderEngine, Ops(FP16), Ops(FP32), Bilinear samples, Flags}},
			// PowerVR Series5
			{ "SGX 530", { Family::SERIES5,   ShaderEngine::USSE1, 8,    8,    1,  0        } },
			{ "SGX 531", { Family::SERIES5,   ShaderEngine::USSE1, 8,    8,    1,  0        } },
			{ "SGX 535", { Family::SERIES5,   ShaderEngine::USSE1, 8,    8,    2,  0        } },
			{ "SGX 540", { Family::SERIES5,   ShaderEngine::USSE1, 16,   16,   2,  0        } },
			{ "SGX 545", { Family::SERIES5,   ShaderEngine::USSE1, 16,   16,   2,  0        } },
			// PowerVR Series5XT
			{ "SGX 543", { Family::SERIES5XT, ShaderEngine::USSE2, 32,   32,   2,  0        } },
			{ "SGX 544", { Family::SERIES5XT, ShaderEngine::USSE2, 32,   32,   2,  0        } },
			{ "SGX 554", { Family::SERIES5XT, ShaderEngine::USSE2, 64,   64,   2,  0        } },
			// PowerVR Series6
			{ "G6200",  { Family::SERIES6,    ShaderEngine::USC,   128,  128,  4,  0        } },
			{ "Han",    { Family::SERIES6,    ShaderEngine::USC,   128,  128,  4,  0        } }, // May be G6200 or G6230
			{ "G6230",  { Family::SERIES6,    ShaderEngine::USC,   192,  128,  4,  CAP_FP16 } },
			{ "G6400",  { Family::SERIES6,    ShaderEngine::USC,   256,  256,  8,  0        } },
			{ "Hood",   { Family::SERIES6,    ShaderEngine::USC,   256,  256,  8,  0        } }, // May be G6400 or G6430
			{ "G6430",  { Family::SERIES6,    ShaderEngine::USC,   384,  256,  8,  CAP_FP16 } },
			{ "G6630",  { Family::SERIES6,    ShaderEngine::USC,   576,  384,  12, CAP_FP16 } },
			// PowerVR Series6XE
			{ "G6020",  { Family::SERIES6XE,  ShaderEngine::USC,   32,   8,    1,  CAP_FP16 } },
			{ "G6050",  { Family::SERIES6XE,  ShaderEngine::USC,   48,   32,   2,  CAP_FP16 } },
			{ "G6060",  { Family::SERIES6XE,  ShaderEngine::USC,   48,   32,   2,  CAP_FP16 } },
			{ "G6100",  { Family::SERIES6XE,  ShaderEngine::USC,   96,   64,   4,  CAP_FP16 } },
			{ "G6110",  { Family::SERIES6XE,  ShaderEngine::USC,   96,   64,   4,  CAP_FP16 } },
			// PowerVR Series6XT
			{ "GX6240", { Family::SERIES6XT,  ShaderEngine::USC,   256,  128,  4,  CAP_FP16 } },
			{ "GX6250", { Family::SERIES6XT,  ShaderEngine::USC,   256,  128,  4,  CAP_FP16 } },
			{ "GX6450", { Family::SERIES6XT,  ShaderEngine::USC,   512,  256,  8,  CAP_FP16 } },
			{ "GX6650", { Family::SERIES6XT,  ShaderEngine::USC,   768,  384,  12, CAP_FP16 } },
			// PowerVR Series7XE
			{ "GE7400", { Family::SERIES7XE,  ShaderEngine::USC,   48,   32,   2,  CAP_FP16 } },
			{ "GE7800", { Family::SERIES7XE,  ShaderEngine::USC,   96,   64,   4,  CAP_FP16 } },
			// PowerVR Series7XT
			{ "GT7200", { Family::SERIES7XT,  ShaderEngine::USC,   256,  128,  4,  CAP_FP16 | CAP_TESSELLATION } },
			{ "GT7400", { Family::SERIES7XT,  ShaderEngine::USC,   512,  256,  8,  CAP_FP16 | CAP_TESSELLATION } },
			{ "GT7600", { Family::SERIES7XT,  ShaderEngine::USC,   768,  384,  12, CAP_FP16 | CAP_TESSELLATION } },
			{ "GT7800", { Family::SERIES7XT,  ShaderEngine::USC,   1024, 512,  16, CAP_FP16 | CAP_TESSELLATION } },
			{ "GT7900", { Family::SERIES7XT,  ShaderEngine::USC,   2048, 1024, 32, CAP_FP16 | CAP_TESSELLATION } },
			// PowerVR Wizard
			{ "GR6500", { Family::SERIES6XT,  ShaderEngine::USC,   512,  256,  8,  CAP_FP16 | CAP_RAYTRACING } }
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
