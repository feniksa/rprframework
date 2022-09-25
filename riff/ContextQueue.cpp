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
        throw std::runtime_error("ContextCommandQueue returned nullptr after create");

    setInstance(std::move(queue));
}

ContextQueue::~ContextQueue() noexcept
{
    const char* errorMsg = "ContextQueue::~ContextQueue error: ";

    try {
        detachAllFilters(true);
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

void ContextQueue::attachFilter(const ImageFilter& filter, const Image& input, Image* output)
{
    assert(filter);
    assert(output);

    if (!output)
        throw std::runtime_error("parameter output should be initialized");

    rif_int status;
    status = rifCommandQueueAttachImageFilter(*this, filter, input, output->instance());
    check(status);

    m_attachedFilters.push_back(&filter);
}

void ContextQueue::detachFilter(const ImageFilter &filter)
{
    auto it = std::find(m_attachedFilters.begin(), m_attachedFilters.end(), &filter);
    if (it == m_attachedFilters.end())
        throw std::runtime_error("Filter not found");

    rif_int status;
    status = rifCommandQueueDetachImageFilter(*this, (*it)->instance());
    check(status);

    m_attachedFilters.erase(it);
}

void ContextQueue::detachAllFilters(bool ignoreErrors)
{
    const char* errorMsg = "Warning: can't detach some filter from queue. Error: ";

    for (const ImageFilter* filter: m_attachedFilters)
    {
        try {
            detachFilter(*filter);
        }
        catch (const std::exception& e) {
            if (ignoreErrors)
                std::cerr << errorMsg << e.what();
            else
                throw;
        }
        catch(...) {
            if (ignoreErrors)
                std::cerr << errorMsg << "Unknown";
            else
                throw;
        }
    }
}

} // namespace