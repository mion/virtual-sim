import sys


class MMU(object):
    """Memory Management Unit"""
    def __init__(self, p_size, num_phys, num_vir):
        self.p_size = p_size
        self.num_phys = num_phys
        self.num_vir = num_vir
        self.p_table = [None for i in range(num_vir)]

    def __str__(self):
        details = 'Page size: {}\n\
                   Physical memory size: {}\n\
                   Virtual memory size: {}\n'.format(self.p_size,\
                                                     self.num_phys,\
                                                     self.num_vir)
        return details + str(self.p_table)

    def map(self, vir_page, phys_page):
        self.p_table[vir_page] = phys_page

    def vir_to_phys(self, addr):
        vir_page = addr / self.p_size 
        offset = addr % self.p_size
        frame = self.p_table[vir_page]
        return frame*self.p_size + offset


def main():
    mmu = MMU(p_size=4*(2**10), num_phys=8, num_vir=16)
    mmu.p_table = [2, 
                   1, 
                   6, 
                   0, 
                   4, 
                   3, 
                   None, 
                   None, 
                   None, 
                   5, 
                   None, 
                   7, 
                   None,
                   None,
                   None,
                   None]

    print mmu
    print '{} to physical: {}'.format(0, mmu.vir_to_phys(0))
    print '{} to physical: {}'.format(8192, mmu.vir_to_phys(8192))
    print '{} to physical: {}'.format(20500, mmu.vir_to_phys(20500))


if __name__ == '__main__':
    main()
