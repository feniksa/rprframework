#include "ContextQueue.h"
#include "Context.h"
#include "Image.h"
#include "ImageFilter.h"
#include <cassert>

namespace riff
{

ContextQueue::ContextQueue(riff::Context &context)
: m_context(context)
{
    assert(m_context);

    rif_int status;
    rif_command_queue queue;

    status = rifContextCreateCommandQueue(m_context, &queue);
    check(status);

    if (!queue)
        throw std::runtime_error("ContextCommandQueue returned nullptr after allocate");

    setInstance(std::move(queue));
}

ContextQueue::~ContextQueue() noexcept
{
    const char* errorMsg = "ContextQueue::~ContextQueue error: ";

    try {
        detachAllFilters();
    } catch (const std::exception& e) {
        std::cerr << errorMsg << e.what();
    }
    catch (...) {
        std::cerr << errorMsg << "Unknown";
    }
}

void ContextQueue::execute()
{
    rif_int status;
    status = rifContextExecuteCommandQueue(m_context, *this,
                                           nullptr, nullptr, nullptr);
    check(status);
}

void ContextQueue::flush()
{
    rif_int status;
    status = rifFlushQueue(*this);
    check(status);
}

void ContextQueue::synchronize()
{
    rif_int status;
    status = rifSyncronizeQueue(*this);
    check(status);
}

void ContextQueue::attachFilter(ImageFilter* filter, const Image* input, Image* output)
{
    assert(filter);
    assert(filter->instance());
    assert(input);
    assert(output);

    if (!output)
        throw std::runtime_error("Output image is not allocated");

    if (!output->instance()) {
        ImageDescription desc = input->getImageInfo();
        output->allocate(desc);
    }

    rif_int status;
    status = rifCommandQueueAttachImageFilter(*this, filter->instance(),
                                              input->instance(), output->instance());
    check(status);

    filter->registerQueue(this);
    m_attachedFilters.push_back(filter);
}

void ContextQueue::detachFilter(rif_image_filter& filter)
{
    assert(filter);

    rif_int status;
    status = rifCommandQueueDetachImageFilter(*this, filter);
    check(status);
}

void ContextQueue::detachFilter(ImageFilter* filter)
{
    auto it = std::find(m_attachedFilters.begin(), m_attachedFilters.end(), filter);
    if (it == m_attachedFilters.end())
        throw std::runtime_error("Filter not found");

    rif_image_filter rawFilter = (*it)->instance();
    assert(rawFilter);

    detachFilter(rawFilter);

    filter->unregisterQueue(this);

    m_attachedFilters.erase(it);
}

void ContextQueue::detachAllFilters()
{
    const char* errorMsg = "Warning: can't detach some filter from queue. Error: ";
    decltype(m_attachedFilters) m_badFilters;
    m_badFilters.reserve(m_attachedFilters.size());

    std::stringstream errorStream;

    for (auto iterator = m_attachedFilters.begin(); iterator != m_attachedFilters.end(); ++iterator)
    {
        ImageFilter* filter = *iterator;
        assert(filter);

        try {
            rif_image_filter rawFilter = (*filter).instance();
            assert(rawFilter);

            detachFilter(rawFilter);
            filter->unregisterQueue(this);
        }
        catch (const std::exception& e) {
             errorStream << errorMsg << e.what();
             m_badFilters.push_back(filter);
        }
        catch(...) {
            errorStream << errorMsg << "Unknown";
            m_badFilters.push_back(filter);
        }
    }
    m_attachedFilters.clear();

    if (!m_badFilters.empty()) {
        std::swap(m_attachedFilters, m_badFilters);
        throw std::runtime_error(errorStream.str());
    }
}

} // namespace