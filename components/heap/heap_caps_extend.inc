/*
 * heap_caps_extend.inc
 *
 *  Created on: 18 Jan 2021
 *      Author: andremaree
 */

/**
 * halMCU_ReportMemory()
 * Based on heap_caps_print_heap_info() from the ESP-IDF
 */
int32_t printfx(const char *, ...) ;

#define	attrRESET			0
#define	attrULINE_ON		4
#define	attrREV_ON			7
#define	attrULINE_OFF		24
#define	attrREV_OFF			27

#define	attrFG_CYAN			36
#define	xpfSGR(a,b,c,d)		(((uint8_t) a << 24) + ((uint8_t) b << 16) + ((uint8_t) c << 8) + (uint8_t) d)

void	halMCU_ReportMemory(uint32_t caps) {
    printfx("%C TotBlk|UsedBlk|FreeBlk| LF_Blk| FromAddr | Size  | Used  | MinFr | Free  |%C0x%04x", xpfSGR(attrFG_CYAN, 0, 0, 0), xpfSGR(attrRESET, 0, 0, 0), caps) ;
    if (caps & MALLOC_CAP_IRAM_8BIT)	printfx(" IRAM-8B") ;
    if (caps & MALLOC_CAP_DEFAULT)		printfx(" Default") ;
    if (caps & MALLOC_CAP_INTERNAL)		printfx(" Internal") ;
    if (caps & MALLOC_CAP_SPIRAM)		printfx(" SPIRAM") ;
    if (caps & MALLOC_CAP_DMA)			printfx(" DMA") ;
    if (caps & MALLOC_CAP_8BIT)			printfx(" 8B") ;
    if (caps & MALLOC_CAP_32BIT)		printfx(" 32B") ;
    if (caps & MALLOC_CAP_EXEC)			printfx(" EXEC") ;
    printfx("\n") ;
    multi_heap_info_t info ;
    heap_t * heap ;
    uint32_t size = 0 ;
    SLIST_FOREACH(heap, &registered_heaps, next) {
        if (heap_caps_match(heap, caps)) {
            multi_heap_get_info(heap->heap, &info) ;
            printfx("%'#7u %'#7u %'#7u %'#7u ",
        		info.total_blocks, info.allocated_blocks, info.free_blocks, info.largest_free_block) ;
            printfx("%p %'#7u %'#7u %'#7u %'#7u\n", heap->start, heap->end - heap->start,
        		info.total_allocated_bytes, info.minimum_free_bytes, info.total_free_bytes) ;
            size += heap->end - heap->start ;
        }
    }
    heap_caps_get_info(&info, caps) ;
    printfx("%C%'#7u %'#7u %'#7u %'#7u ", xpfSGR(attrFG_CYAN, 0, 0, 0), info.total_blocks,
    		info.allocated_blocks, info.free_blocks, info.largest_free_block) ;
    printfx("%C==Totals==%C", xpfSGR(attrREV_ON, 0, 0, 0), xpfSGR(attrREV_OFF, 0, 0, 0)) ;
    printfx(" %'#7u %'#7u %'#7u %'#7u%C\n\n", size, info.total_allocated_bytes,
    		info.minimum_free_bytes, info.total_free_bytes, xpfSGR(attrRESET, 0, 0, 0)) ;
}
