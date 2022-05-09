# GPL3, Copyright (c) Max Hofheinz, UdeS, 2021

import numpy, fiddle

def curl_E(E):
    curl_E = numpy.zeros(E.shape)
    curl_E[:, :-1, :, 0] += E[:, 1:, :, 2] - E[:, :-1, :, 2]
    curl_E[:, :, :-1, 0] -= E[:, :, 1:, 1] - E[:, :, :-1, 1]

    curl_E[:, :, :-1, 1] += E[:, :, 1:, 0] - E[:, :, :-1, 0]
    curl_E[:-1, :, :, 1] -= E[1:, :, :, 2] - E[:-1, :, :, 2]

    curl_E[:-1, :, :, 2] += E[1:, :, :, 1] - E[:-1, :, :, 1]
    curl_E[:, :-1, :, 2] -= E[:, 1:, :, 0] - E[:, :-1, :, 0]
    return curl_E

def curl_H(H):
    curl_H = numpy.zeros(H.shape)

    curl_H[:,1:,:,0] += H[:,1:,:,2] - H[:,:-1,:,2]
    curl_H[:,:,1:,0] -= H[:,:,1:,1] - H[:,:,:-1,1]

    curl_H[:,:,1:,1] += H[:,:,1:,0] - H[:,:,:-1,0]
    curl_H[1:,:,:,1] -= H[1:,:,:,2] - H[:-1,:,:,2]

    curl_H[1:,:,:,2] += H[1:,:,:,1] - H[:-1,:,:,1]
    curl_H[:,1:,:,2] -= H[:,1:,:,0] - H[:,:-1,:,0]
    return curl_H


def timestep(E, H, courant_number, source_pos, source_val):
    E += courant_number * curl_H(H)
    E[source_pos] += source_val
    H -= courant_number * curl_E(E)
    return E, H


class WaveEquation:
    def __init__(self, s, courant_number, source):
        s = s + (3,)
        self.E = numpy.zeros(s)
        self.H = numpy.zeros(s)
        self.courant_number = courant_number
        self.source = source
        self.index = 0

    def __call__(self, figure, field_component, slice, slice_index, initial=False):
        if field_component < 3:
            field = self.E
        else:
            field = self.H
            field_component = field_component % 3
        if slice == 0:
            field = field[slice_index, :, :, field_component]
        elif slice == 1:
            field = field[:, slice_index, :, field_component]
        elif slice == 2:
            field = field[:, :, slice_index, field_component]
        source_val, source_pos = source(self.index)
        self.E, self.H = timestep(self.E, self.H, self.courant_number, source_val, source_pos)

        if initial:
            axes = figure.add_subplot(111)
            self.image = axes.imshow(field, vmin=-1e-2, vmax=1e-2)
        else:
            self.image.set_data(field)
        self.index += 1


# if __name__ == "__main__":
#     n = 100
#     r = 0.01
#     l = 30


#     def source(index):
#         return ([n // 3], [n // 3], [n // 2],[0]), 0.1*numpy.sin(0.1 * index)


#     w = WaveEquation((n, n, n), 0.1, source)
#     fiddle.fiddle(w, [('field component',{'Ex':0,'Ey':1,'Ez':2, 'Hx':3,'Hy':4,'Hz':5}),('slice',{'XY':2,'YZ':0,'XZ':1}),('slice index',0,n-1,n//2,1)], update_interval=0.01)

if __name__ == "__main__":
    data = numpy.zeros((2, 2, 2, 3))
    data[0][0][0][0] = 1.0
    data[0][1][0][0] = 2.0
    data[1][0][0][0] = 3.0
    data[1][1][0][0] = 4.0
    data[0][0][1][0] = 5.0
    data[0][1][1][0] = 6.0
    data[1][0][1][0] = 7.0
    data[1][1][1][0] = 8.0
    data[0][0][0][1] = 9.0
    data[0][1][0][1] = 10.0
    data[1][0][0][1] = 11.0
    data[1][1][0][1] = 12.0
    data[0][0][1][1] = 13.0
    data[0][1][1][1] = 14.0
    data[1][0][1][1] = 15.0
    data[1][1][1][1] = 16.0
    data[0][0][0][2] = 17.0
    data[0][1][0][2] = 18.0
    data[1][0][0][2] = 19.0
    data[1][1][0][2] = 20.0
    data[0][0][1][2] = 21.0
    data[0][1][1][2] = 22.0
    data[1][0][1][2] = 23.0
    data[1][1][1][2] = 24.0
    # print(data)
    c_E = curl_E(data)
    
    print(c_E)
