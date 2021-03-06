#include "ring_buffer.h"
#include "retarget.h"
#include "stm32f4xx.h"
#include "cmsis_os.h"
#include <stdlib.h>
#include <string.h>

//判断x是否是2的次方
#define is_power_of_2(x) ((x) != 0 && (((x) & ((x)-1)) == 0))
//取a和b中最小值
#define rb_min(a, b) (((a) < (b)) ? (a) : (b))

osMutexDef(rb_lock);

//初始化缓冲区
ring_buffer_s *__ring_buffer_init(void *buffer, uint32_t size)
{
    ring_buffer_s *rb = NULL;
    if (!is_power_of_2(size))
    {
        printf_safe("size must be power of 2.\n");
        return rb;
    }
    rb = (ring_buffer_s *)malloc(sizeof(ring_buffer_s));
    if (!rb)
    {
        printf_safe("Failed to malloc memory.\n");
        return rb;
    }
    memset(rb, 0, sizeof(ring_buffer_s));
    rb->buffer = buffer;
    rb->size = size;
    rb->in = 0;
    rb->out = 0;
    rb->lock = osMutexCreate(osMutex(rb_lock));
    return rb;
}
ring_buffer_s *ring_uint8_init(uint32_t size)
{
    uint8_t *buffer;
    buffer = (uint8_t *)malloc(size);
    if (buffer == NULL)
    {
        return NULL;
    }
    memset(buffer, 0, size);
    return __ring_buffer_init(buffer, size);
}
ring_buffer_s *ring_uint16_init(uint32_t size)
{
    uint16_t *buffer;
    buffer = (uint16_t *)malloc(size * sizeof(uint16_t));
    if (buffer == NULL)
    {
        return NULL;
    }
    memset(buffer, 0, size * sizeof(uint16_t));
    return __ring_buffer_init(buffer, size);
}
ring_buffer_s *ring_double_init(uint32_t size)
{
    double *buffer;
    buffer = (double *)malloc(size * sizeof(double));
    if (buffer == NULL)
    {
        return NULL;
    }
    memset(buffer, 0, size * sizeof(double));
    return __ring_buffer_init(buffer, size);
}
//释放缓冲区
void ring_buffer_free(ring_buffer_s *rb)
{
    if (rb)
    {
        if (rb->buffer)
        {
            free(rb->buffer);
            rb->buffer = NULL;
        }
        free(rb);
        rb = NULL;
    }
}
//缓冲区的长度
uint32_t ring_buffer_len(const ring_buffer_s *rb)
{
    return (rb->in - rb->out);
}
//从缓冲区中取数据
uint32_t ring_buffer_get(ring_buffer_s *rb, void *buffer, uint32_t len)
{
    uint32_t l = 0;
    len = rb_min(len, rb->in - rb->out);
    /* first get the data from fifo->out until the end of the buffer */
    l = rb_min(len, rb->size - (rb->out & (rb->size - 1)));
    memcpy(buffer, rb->buffer + (rb->out & (rb->size - 1)), l);
    /* then get the rest (if any) from the beginning of the buffer */
    memcpy(buffer + l, rb->buffer, len - l);
    rb->out += len; //每次累加，到达最大值后溢出，自动转为0
    return len;
}
uint32_t ring_uint16_get(ring_buffer_s *rb, void *buffer, uint32_t len)
{
    uint32_t l = 0;
    len = rb_min(len, rb->in - rb->out);
    /* first get the data from fifo->out until the end of the buffer */
    l = rb_min(len, rb->size - (rb->out & (rb->size - 1)));
    memcpy(buffer, (uint16_t *)(rb->buffer) + (rb->out & (rb->size - 1)), l * sizeof(uint16_t));
    /* then get the rest (if any) from the beginning of the buffer */
    memcpy((uint16_t *)buffer + l, rb->buffer, (len - l) * sizeof(uint16_t));
    rb->out += len;   //每次累加，到达最大值后溢出，自动转为0
    return len;
}
uint32_t ring_double_get(ring_buffer_s *rb, void *buffer, uint32_t len)
{
    uint32_t l = 0;
    len = rb_min(len, rb->in - rb->out);
    /* first get the data from fifo->out until the end of the buffer */
    l = rb_min(len, rb->size - (rb->out & (rb->size - 1)));
    memcpy(buffer, (double *)(rb->buffer) + (rb->out & (rb->size - 1)), l * sizeof(double));
    /* then get the rest (if any) from the beginning of the buffer */
    memcpy((double *)buffer + l, rb->buffer, (len - l) * sizeof(double));
    rb->out += len;  //每次累加，到达最大值后溢出，自动转为0
    return len;
}
//向缓冲区中存放数据
uint32_t ring_buffer_put(ring_buffer_s *rb, void *buffer, uint32_t len)
{
    uint32_t l = 0;
    len = rb_min(len, rb->size - rb->in + rb->out);
    /* first put the data starting from fifo->in to buffer end */
    l = rb_min(len, rb->size - (rb->in & (rb->size - 1)));
    memcpy(rb->buffer + (rb->in & (rb->size - 1)), buffer, l);
    /* then put the rest (if any) at the beginning of the buffer */
    memcpy(rb->buffer, buffer + l, len - l);
    rb->in += len; //每次累加，到达最大值后溢出，自动转为0
    return len;
}
uint32_t ring_uint16_put(ring_buffer_s *rb, void *buffer, uint32_t len)
{
    uint32_t l = 0;
    len = rb_min(len, rb->size - rb->in + rb->out);
    /* first put the data starting from fifo->in to buffer end */
    l = rb_min(len, rb->size - (rb->in & (rb->size - 1)));
    memcpy((uint16_t *)(rb->buffer) + (rb->in & (rb->size - 1)), buffer, l * sizeof(uint16_t));
    /* then put the rest (if any) at the beginning of the buffer */
    memcpy(rb->buffer, (uint16_t *)buffer + l, (len - l) * sizeof(uint16_t));
    rb->in += len;   //每次累加，到达最大值后溢出，自动转为0
    return len;
}
uint32_t ring_double_put(ring_buffer_s *rb, void *buffer, uint32_t len)
{
    uint32_t l = 0;
    len = rb_min(len, rb->size - rb->in + rb->out);
    /* first put the data starting from fifo->in to buffer end */
    l = rb_min(len, rb->size - (rb->in & (rb->size - 1)));
    memcpy((double*)(rb->buffer) + (rb->in & (rb->size - 1)), buffer, l * sizeof(double));
    /* then put the rest (if any) at the beginning of the buffer */
    memcpy(rb->buffer, (double*)buffer + l, (len - l) * sizeof(double));
    rb->in += len;  //每次累加，到达最大值后溢出，自动转为0
    return len;
}

//uint32_t ring_buffer_len(const ring_buffer_s *rb)
//{
//    uint32_t len = 0;
//    osMutexWait(rb->lock, osWaitForever);
//    len = __ring_buffer_len(rb);
//    osMutexRelease(rb->lock);
//    return len;
//}
//
//uint32_t ring_buffer_get(ring_buffer_s *rb, void *buffer, uint32_t len)
//{
//    uint32_t ret;
//    osMutexWait(rb->lock, osWaitForever);
//    ret = __ring_buffer_get(rb, buffer, len);
//    //buffer中没有数据
//    if (rb->in == rb->out)
//        rb->in = rb->out = 0;
//    osMutexRelease(rb->lock);
//    return ret;
//}
//
//uint32_t ring_buffer_put(ring_buffer_s *rb, void *buffer, uint32_t len)
//{
//    uint32_t ret;
//    osMutexWait(rb->lock, osWaitForever);
//    ret = __ring_buffer_put(rb, buffer, len);
//    osMutexRelease(rb->lock);
//    return ret;
//}
