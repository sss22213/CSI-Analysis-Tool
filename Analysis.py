from subprocess import check_output
import numpy as np
import matplotlib.pyplot as plt
from plot_CSI import *
from matplotlib.mlab import PCA

class CSI_get:
    def __init__(self,path):
        self.Path = "\""+path+"\""

    def Check_Packet_Count(self):
        cmd = "main "+self.Path+" "+str(1)+" Num"
        return int(check_output(cmd, shell=True))

    def Check_Effection_Packet(self):
        Eff_Packet = []
        Packet_Number = 1
        while True:
            if self.Check_effection(Packet_Number):
                print("OK")
                break
            Packet_Number = Packet_Number + 1
            Eff_Packet.append(Packet_Number)
        return Eff_Packet

    def Check_effection(self,Packet_Number):
        cmd = "main "+self.Path+" "+str(Packet_Number)+" Bfee_count"
        if check_output(cmd, shell=True)==b'1':
            return 1
        else:
            return 0
    def Get_Bfee_count(self,Packet_Number):
        cmd = "main "+self.Path+" "+str(Packet_Number)+" Bfee_count"
        if self.Check_effection(Packet_Number):
            return 1
        return int(check_output(cmd, shell=True))

    def Get_Perm(self,Packet_Number):
        cmd = "main "+self.Path+" "+str(Packet_Number)+" Perm"
        if self.Check_effection(Packet_Number):
            return 1
        result = str(check_output(cmd, shell=True))
        return [int(result[2]),int(result[4]),int(result[6])]

    def Get_Nrx(self,Packet_Number):
        cmd = "main "+self.Path+" "+str(Packet_Number)+" Nrx"
        if self.Check_effection(Packet_Number):
            return 1
        return int(check_output(cmd, shell=True))

    def Get_Ntx(self,Packet_Number):
        cmd = "main "+self.Path+" "+str(Packet_Number)+" Ntx"
        if self.Check_effection(Packet_Number):
            return 1
        return int(check_output(cmd, shell=True))

    def Get_Noise(self,Packet_Number):
        cmd = "main "+self.Path+" "+str(Packet_Number)+" Noise"
        if self.Check_effection(Packet_Number):
            return 1
        return int(check_output(cmd, shell=True))

    def Get_RSSI(self,Packet_Number):
        cmd = "main "+self.Path+" "+str(Packet_Number)+" RSSI"
        if self.Check_effection(Packet_Number):
            return 1
        result = str(check_output(cmd, shell=True))
        pos1 = result.find(',')
        pos2 = result.find(',', pos1+1)
        pos3 = result.find(',', pos2+1)
        return [int(result[2:pos1]),int(result[pos1+1:pos2]),int(result[pos2+1:pos3])]

    def Get_CSI(self,Packet_Number):
        cmd = "main "+self.Path+" "+str(Packet_Number)+" CSI"
        if self.Check_effection(Packet_Number):
            return 1
        result = str(check_output(cmd, shell=True))
        if(len(result) < self.Get_Nrx(Packet_Number)*self.Get_Ntx(Packet_Number)*30):
            return "CSI size is error"
        pos1 = 0
        pos2 = 0
        pos3 = 0
        buff = []
        buff2 = []
        buff3 = []
        CSI_Box = []
        for index in range(180):
            pos3 = result.find('|',pos3+1)
            pos2 = result.find(',',pos2+1)
            pos1 = result.find('$',pos1+1)
            buff.append(pos1)
            buff2.append(pos2)
            buff3.append(pos3)
        count = 0
        for index in buff:
            prv = buff[count]
            mid = buff2[count]
            tail = buff3[count]
            count = count + 1
            CSI_Box.append(complex(int(result[prv+1:mid]),int(result[mid+1:tail])))
        return CSI_Box
    
    def Complete_Format(self,Packet_Number):
        if self.Check_effection(Packet_Number):
            return 1
        Bfee_count = self.Get_Bfee_count(Packet_Number)
        Perm = self.Get_Perm(Packet_Number)
        Nrx = self.Get_Nrx(Packet_Number)
        Ntx = self.Get_Ntx(Packet_Number)
        Noise = self.Get_Noise(Packet_Number)
        RSSI = self.Get_RSSI(Packet_Number)
        CSI = np.sort_complex(np.zeros((30,3,2)))
        #self.CSI = np.complex(self.CSI)
        CSI_Packet = self.Get_CSI(Packet_Number)
        if(len(CSI_Packet) < 20):
            return CSI_Packet
        count = 0
        for Subcarrier in range(30):
            for Nrx in range(3):
                for Ntx in range(2):
                    CSI[Subcarrier,Nrx,Ntx] = CSI_Packet[count]
                    count = count + 1
        return [Bfee_count,Perm,Nrx,Ntx,Noise,RSSI,CSI]

    def Muilt_data(self,L_Range,H_Rang):
        Box = []
        buff = 0
        for Packet_number in range(H_Rang-L_Range):
            if(self.Get_Bfee_count(Packet_number) != buff):
                Box.append(self.Complete_Format(Packet_number))
                buff = self.Get_Bfee_count(Packet_number)
        return Box

if __name__ == '__main__':
    CSI = CSI_get("0537_6011_1.dat")
    print(CSI.Get_Bfee_count(666))
   