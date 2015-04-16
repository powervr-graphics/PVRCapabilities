#pragma once

#include <string>

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
	PVR_CAP_FP16            = (1<<1),        // USC has 16-bit float units
	PVR_CAP_RAYTRACING      = (1<<2),        // Accelerated ray tracing
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
	bool DetermineRogueCapabilities(const std::string renderer_string, SCapabilities& capabilities) {
		if(std::string::npos == renderer_string.find("Rogue")) return false;

		SCapabilities tmp_capabilities;
		capabilities = tmp_capabilities;
		// PowerVR Series6
		if((std::string::npos != renderer_string.find("G6200"))
		   || (std::string::npos != renderer_string.find("Han"))) {
			// NOTE: Han may be a G6230. Assume G6200, as we can't be sure
			tmp_capabilities.generation = PVR_SERIES6;
			tmp_capabilities.fp16_operations_per_clock = 128;
			tmp_capabilities.fp32_operations_per_clock = 128;
			tmp_capabilities.bilinear_samples_per_clock = 4;
		}
		else if(std::string::npos != renderer_string.find("G6230")) {
			tmp_capabilities.generation = PVR_SERIES6;
			tmp_capabilities.fp16_operations_per_clock = 192;
			tmp_capabilities.fp32_operations_per_clock = 128;
			tmp_capabilities.bilinear_samples_per_clock = 4;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		else if((std::string::npos != renderer_string.find("G6400"))
			|| (std::string::npos != renderer_string.find("Hood"))) {
			// NOTE: Hood may be a G6430. Assume G6400, as we can't be sure
			tmp_capabilities.generation = PVR_SERIES6;
			tmp_capabilities.fp16_operations_per_clock = 256;
			tmp_capabilities.fp32_operations_per_clock = 256;
			tmp_capabilities.bilinear_samples_per_clock = 8;
		}
		else if(std::string::npos != renderer_string.find("G6430")) {
			tmp_capabilities.generation = PVR_SERIES6;
			tmp_capabilities.fp16_operations_per_clock = 384;
			tmp_capabilities.fp32_operations_per_clock = 256;
			tmp_capabilities.bilinear_samples_per_clock = 8;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		else if(std::string::npos != renderer_string.find("G6630")) {
			tmp_capabilities.generation = PVR_SERIES6;
			tmp_capabilities.fp16_operations_per_clock = 576;
			tmp_capabilities.fp32_operations_per_clock = 384;
			tmp_capabilities.bilinear_samples_per_clock = 12;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		// PowerVR Series6XE
		else if(std::string::npos != renderer_string.find("G6020")) {
			tmp_capabilities.generation = PVR_SERIES6XE;
			tmp_capabilities.fp16_operations_per_clock = 32;
			tmp_capabilities.fp32_operations_per_clock = 8;
			tmp_capabilities.bilinear_samples_per_clock = 1;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		else if(std::string::npos != renderer_string.find("G6050")) {
			tmp_capabilities.generation = PVR_SERIES6XE;
			tmp_capabilities.fp16_operations_per_clock = 48;
			tmp_capabilities.fp32_operations_per_clock = 32;
			tmp_capabilities.bilinear_samples_per_clock = 2;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		else if(std::string::npos != renderer_string.find("G6060")) {
			tmp_capabilities.generation = PVR_SERIES6XE;
			tmp_capabilities.fp16_operations_per_clock = 48;
			tmp_capabilities.fp32_operations_per_clock = 32;
			tmp_capabilities.bilinear_samples_per_clock = 2;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		else if(std::string::npos != renderer_string.find("G6100")) {
			tmp_capabilities.generation = PVR_SERIES6XE;
			tmp_capabilities.fp16_operations_per_clock = 96;
			tmp_capabilities.fp32_operations_per_clock = 64;
			tmp_capabilities.bilinear_samples_per_clock = 4;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		else if(std::string::npos != renderer_string.find("G6110")) {
			tmp_capabilities.generation = PVR_SERIES6XE;
			tmp_capabilities.fp16_operations_per_clock = 96;
			tmp_capabilities.fp32_operations_per_clock = 64;
			tmp_capabilities.bilinear_samples_per_clock = 4;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		// PowerVR Series6XT
		else if(std::string::npos != renderer_string.find("GX6240")) {
			tmp_capabilities.generation = PVR_SERIES6XT;
			tmp_capabilities.fp16_operations_per_clock = 256;
			tmp_capabilities.fp32_operations_per_clock = 128;
			tmp_capabilities.bilinear_samples_per_clock = 4;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		else if(std::string::npos != renderer_string.find("GX6250")) {
			tmp_capabilities.generation = PVR_SERIES6XT;
			tmp_capabilities.fp16_operations_per_clock = 256;
			tmp_capabilities.fp32_operations_per_clock = 128;
			tmp_capabilities.bilinear_samples_per_clock = 4;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		else if(std::string::npos != renderer_string.find("GX6450")) {
			tmp_capabilities.generation = PVR_SERIES6XT;
			tmp_capabilities.fp16_operations_per_clock = 512;
			tmp_capabilities.fp32_operations_per_clock = 256;
			tmp_capabilities.bilinear_samples_per_clock = 8;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		else if(std::string::npos != renderer_string.find("GX6650")) {
			tmp_capabilities.generation = PVR_SERIES6XT;
			tmp_capabilities.fp16_operations_per_clock = 768;
			tmp_capabilities.fp32_operations_per_clock = 384;
			tmp_capabilities.bilinear_samples_per_clock = 12;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		// PowerVR Series7XE
		else if(std::string::npos != renderer_string.find("GE7400")) {
			tmp_capabilities.generation = PVR_SERIES7XE;
			tmp_capabilities.fp16_operations_per_clock = 48;
			tmp_capabilities.fp32_operations_per_clock = 32;
			tmp_capabilities.bilinear_samples_per_clock = 2;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		else if(std::string::npos != renderer_string.find("GE7800")) {
			tmp_capabilities.generation = PVR_SERIES7XE;
			tmp_capabilities.fp16_operations_per_clock = 96;
			tmp_capabilities.fp32_operations_per_clock = 64;
			tmp_capabilities.bilinear_samples_per_clock = 4;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		// PowerVR Series7XT
		else if(std::string::npos != renderer_string.find("GT7200")) {
			tmp_capabilities.generation = PVR_SERIES7XT;
			tmp_capabilities.fp16_operations_per_clock = 256;
			tmp_capabilities.fp32_operations_per_clock = 128;
			tmp_capabilities.bilinear_samples_per_clock = 4;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		else if(std::string::npos != renderer_string.find("GT7400")) {
			tmp_capabilities.generation = PVR_SERIES7XT;
			tmp_capabilities.fp16_operations_per_clock = 512;
			tmp_capabilities.fp32_operations_per_clock = 256;
			tmp_capabilities.bilinear_samples_per_clock = 8;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		else if(std::string::npos != renderer_string.find("GT7600")) {
			tmp_capabilities.generation = PVR_SERIES7XT;
			tmp_capabilities.fp16_operations_per_clock = 768;
			tmp_capabilities.fp32_operations_per_clock = 384;
			tmp_capabilities.bilinear_samples_per_clock = 12;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		else if(std::string::npos != renderer_string.find("GT7800")) {
			tmp_capabilities.generation = PVR_SERIES7XT;
			tmp_capabilities.fp16_operations_per_clock = 1024;
			tmp_capabilities.fp32_operations_per_clock = 512;
			tmp_capabilities.bilinear_samples_per_clock = 16;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		else if(std::string::npos != renderer_string.find("GT7900")) {
			tmp_capabilities.generation = PVR_SERIES7XT;
			tmp_capabilities.fp16_operations_per_clock = 2048;
			tmp_capabilities.fp32_operations_per_clock = 1024;
			tmp_capabilities.bilinear_samples_per_clock = 32;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		// PowerVR Wizard
		else if(std::string::npos != renderer_string.find("GR6500")) {
			tmp_capabilities.generation = PVR_SERIES6XT;
			tmp_capabilities.fp16_operations_per_clock = 512;
			tmp_capabilities.fp32_operations_per_clock = 256;
			tmp_capabilities.bilinear_samples_per_clock = 8;
			tmp_capabilities.flags |= PVR_CAP_FP16;
			tmp_capabilities.flags |= PVR_CAP_RAYTRACING;
		}
		else{
			return false;
		}
		capabilities = tmp_capabilities;
		return true;
	}
}
