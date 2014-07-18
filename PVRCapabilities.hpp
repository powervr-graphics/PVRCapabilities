#pragma once

#include <string>

namespace pvr {
enum EGPUGeneration {
	PVR_SERIES6,
	PVR_SERIES6XE,
	PVR_SERIES6XT,
	PVR_SERIES_UNKNOWN
};
enum ECapabilityFlags {
	PVR_CAP_FP16                 = (1<<1), // USC has 16-bit float units
	PVR_CAP_LOSSLESS_FRAMEBUFFER = (1<<2), // Lossless framebuffer compression
	PVR_CAP_LOSSLESS_GEOMETRY    = (1<<3), // Lossless Parameter Buffer geometry compression
	/* NOTE
	You must query for ASTC driver support with GL_OES_texture_compression_astc, 
	GL_KHR_texture_compression_astc_hdr or GL_KHR_texture_compression_astc_ldr
	*/
	PVR_CAP_ASTC                 = (1<<4), // Hardware support for ASTC decompression
};
struct SCapabilities {
	EGPUGeneration generation;             // Series6 variant
	float num_clusters;                    // Number of USC units
	unsigned int num_usc_pipelines;        // Number of pipelines within each USC
	unsigned int flags;                    // Capability flags
	
	SCapabilities()
	:
	generation(PVR_SERIES_UNKNOWN),
	num_clusters(0.0f),
	num_usc_pipelines(16),
	flags(PVR_CAP_LOSSLESS_GEOMETRY)
	{}
};
	/*!****************************************************************************
	 * @function        DetermineCapabilities
	 * @brief           Conservative estimate of a PowerVR Series6 GPU's capabilities.
	 *                  "Han" and "Hood" cases handle legacy GL_RENDERER string formatting
	 *
	 * @param[in]       gpu_model         GL_RENDERER string value
	 * @param[out]      capabilities      Struct containing GPU capabilities
	 * @return          bool              True if the model number is recognized
	 ******************************************************************************/
	bool DetermineSeries6Capabilities(const std::string gpu_model, SCapabilities& capabilities) {
		SCapabilities tmp_capabilities;
		capabilities = tmp_capabilities;
		if((std::string::npos != gpu_model.find("G6200"))
		   || (std::string::npos != gpu_model.find("Han"))) {
			// NOTE: Han may be a G6230. Assume G6200, as we can't be sure
			tmp_capabilities.generation = PVR_SERIES6;
			tmp_capabilities.num_clusters = 2.0f;
		}
		else if(std::string::npos != gpu_model.find("G6230")) {
			tmp_capabilities.generation = PVR_SERIES6;
			tmp_capabilities.num_clusters = 2.0f;
			tmp_capabilities.flags |= PVR_CAP_FP16;
			tmp_capabilities.flags |= PVR_CAP_LOSSLESS_FRAMEBUFFER;
		}
		else if((std::string::npos != gpu_model.find("G6400"))
			|| (std::string::npos != gpu_model.find("Hood"))) {
			// NOTE: Hood may be a G6430. Assume G6400, as we can't be sure
			tmp_capabilities.generation = PVR_SERIES6;
			tmp_capabilities.num_clusters = 4.0f;
		}
		else if(std::string::npos != gpu_model.find("G6430")) {
			tmp_capabilities.generation = PVR_SERIES6;
			tmp_capabilities.num_clusters = 4.0f;
			tmp_capabilities.flags |= PVR_CAP_FP16;
			tmp_capabilities.flags |= PVR_CAP_LOSSLESS_FRAMEBUFFER;
		}
		else if(std::string::npos != gpu_model.find("G6630")) {
			tmp_capabilities.generation = PVR_SERIES6;
			tmp_capabilities.num_clusters = 6.0f;
			tmp_capabilities.flags |= PVR_CAP_FP16;
			tmp_capabilities.flags |= PVR_CAP_LOSSLESS_FRAMEBUFFER;
		}
		else if(std::string::npos != gpu_model.find("G6050")) {
			tmp_capabilities.generation = PVR_SERIES6XE;
			tmp_capabilities.num_clusters = 0.5f;
			tmp_capabilities.num_usc_pipelines = 8;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		else if(std::string::npos != gpu_model.find("G6060")) {
			tmp_capabilities.generation = PVR_SERIES6XE;
			tmp_capabilities.num_clusters = 0.5f;
			tmp_capabilities.num_usc_pipelines = 8;
			tmp_capabilities.flags |= PVR_CAP_FP16;
			tmp_capabilities.flags |= PVR_CAP_LOSSLESS_FRAMEBUFFER;
		}
		else if(std::string::npos != gpu_model.find("G6100")) {
			tmp_capabilities.generation = PVR_SERIES6XE;
			tmp_capabilities.num_clusters = 1.0f;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		else if(std::string::npos != gpu_model.find("G6110")) {
			tmp_capabilities.generation = PVR_SERIES6XE;
			tmp_capabilities.num_clusters = 1.0f;
			tmp_capabilities.flags |= PVR_CAP_FP16;
			tmp_capabilities.flags |= PVR_CAP_LOSSLESS_FRAMEBUFFER;
		}
		else if(std::string::npos != gpu_model.find("GX6240")) {
			tmp_capabilities.generation = PVR_SERIES6XT;
			tmp_capabilities.num_clusters = 2.0f;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		else if(std::string::npos != gpu_model.find("GX6250")) {
			tmp_capabilities.generation = PVR_SERIES6XT;
			tmp_capabilities.num_clusters = 2.0f;
			tmp_capabilities.flags |= PVR_CAP_FP16;
			tmp_capabilities.flags |= PVR_CAP_LOSSLESS_FRAMEBUFFER;
			tmp_capabilities.flags |= PVR_CAP_ASTC;
		}
		else if(std::string::npos != gpu_model.find("GX6450")) {
			tmp_capabilities.generation = PVR_SERIES6XT;
			tmp_capabilities.num_clusters = 4.0f;
			tmp_capabilities.flags |= PVR_CAP_FP16;
			tmp_capabilities.flags |= PVR_CAP_LOSSLESS_FRAMEBUFFER;
			tmp_capabilities.flags |= PVR_CAP_ASTC;
		}
		else if(std::string::npos != gpu_model.find("GX6650")) {
			tmp_capabilities.generation = PVR_SERIES6XT;
			tmp_capabilities.num_clusters = 6.0f;
			tmp_capabilities.flags |= PVR_CAP_FP16;
			tmp_capabilities.flags |= PVR_CAP_LOSSLESS_FRAMEBUFFER;
			tmp_capabilities.flags |= PVR_CAP_ASTC;
		}
		else{
			return false;
		}
		capabilities = tmp_capabilities;
		return true;
	}
}