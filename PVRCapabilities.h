#ifndef __PVR_CAPABILITIES_H__
#define __PVR_CAPABILITIES_H__

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
	PVR_CAP_LOSSLESS_COMPRESSION = (1<<2), // Lossless PB & framebuffer compression
};

struct SCapabilities {
	EGPUGeneration generation;             // Series6 variant
	float num_clusters;                    // Number of USC units
	unsigned int flags;                    // Capability flags
	
	SCapabilities()
	:
	generation(PVR_SERIES_UNKNOWN),
	num_clusters(0.0f),
	flags(0)
	{}
};

	/*!****************************************************************************
	 * @function        DetermineCapabilities
	 * @brief           Conservative estimate of a PowerVR Series6 GPU's capabilities.
	 *                  "Han" and "Hood" handle legacy GL_RENDERER string cases
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
			tmp_capabilities.flags |= PVR_CAP_LOSSLESS_COMPRESSION;
		}
		else if(std::string::npos != gpu_model.find("G6230")) {
			tmp_capabilities.generation = PVR_SERIES6;
			tmp_capabilities.num_clusters = 2.0f;
			tmp_capabilities.flags |= PVR_CAP_FP16;
			tmp_capabilities.flags |= PVR_CAP_LOSSLESS_COMPRESSION;
		}
		else if((std::string::npos != gpu_model.find("G6400"))
				|| (std::string::npos != gpu_model.find("Hood"))) {
			// NOTE: Hood may be a G6430. Assume G6400, as we can't be sure
			tmp_capabilities.generation = PVR_SERIES6;
			tmp_capabilities.num_clusters = 4.0f;
			tmp_capabilities.flags |= PVR_CAP_LOSSLESS_COMPRESSION;
		}
		else if(std::string::npos != gpu_model.find("G6430")) {
			tmp_capabilities.generation = PVR_SERIES6;
			tmp_capabilities.num_clusters = 4.0f;
			tmp_capabilities.flags |= PVR_CAP_FP16;
			tmp_capabilities.flags |= PVR_CAP_LOSSLESS_COMPRESSION;
		}
		else if(std::string::npos != gpu_model.find("G6050")) {
			tmp_capabilities.generation = PVR_SERIES6XE;
			tmp_capabilities.num_clusters = 0.5f;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		else if(std::string::npos != gpu_model.find("G6060")) {
			tmp_capabilities.generation = PVR_SERIES6XE;
			tmp_capabilities.num_clusters = 0.5f;
			tmp_capabilities.flags |= PVR_CAP_FP16;
			tmp_capabilities.flags |= PVR_CAP_LOSSLESS_COMPRESSION;
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
			tmp_capabilities.flags |= PVR_CAP_LOSSLESS_COMPRESSION;
		}
		else if(std::string::npos != gpu_model.find("GX6250")) {
			tmp_capabilities.generation = PVR_SERIES6XT;
			tmp_capabilities.num_clusters = 2.0f;
			tmp_capabilities.flags |= PVR_CAP_FP16;
			tmp_capabilities.flags |= PVR_CAP_LOSSLESS_COMPRESSION;
		}
		else if(std::string::npos != gpu_model.find("GX6450")) {
			tmp_capabilities.generation = PVR_SERIES6XT;
			tmp_capabilities.num_clusters = 4.0f;
			tmp_capabilities.flags |= PVR_CAP_FP16;
			tmp_capabilities.flags |= PVR_CAP_LOSSLESS_COMPRESSION;
		}
		else if(std::string::npos != gpu_model.find("GX6650")) {
			tmp_capabilities.generation = PVR_SERIES6XT;
			tmp_capabilities.num_clusters = 6.0f;
			tmp_capabilities.flags |= PVR_CAP_FP16;
			tmp_capabilities.flags |= PVR_CAP_LOSSLESS_COMPRESSION;
		}
		else if(std::string::npos != gpu_model.find("GX6240")) {
			tmp_capabilities.generation = PVR_SERIES6XT;
			tmp_capabilities.num_clusters = 2.0f;
			tmp_capabilities.flags |= PVR_CAP_FP16;
		}
		else{
			return false;
		}
		capabilities = tmp_capabilities;
		return true;
	}
}

#endif // __PVR_CAPABILITIES_H__
