from Analysis import *
import matplotlib.pyplot as plt

class plot_CSI:
    def __init__(self,path):
        self.Path = path
        self.CSI = CSI_get(self.Path)

    def plot_CSI_six(self,Packet):
        self.plot_CSI_six_animation([Packet,Packet+1,1],'slow')
    '''
    Speed:
        Slow: 1
        Common: 0.1
        Fast: 0.01
    Packet_range:
        [low,high]
    '''
    def plot_CSI_six_animation(self,Packet_range,Speed):
        time = []
        point1 = []
        point2 = []
        point3 = []
        point4 = []
        point5 = []
        point6 = []

        for time_index in range(1,31,1):
                time.append(time_index)
        #Setting Speed
        if Speed == 'Slow':
            speed = 1
        elif Speed == 'Common':
            speed = 0.1
        elif Speed == 'Fast':
            speed = 0.01
        else: 
            speed = 1

        #Setting packet range
        low,high = Packet_range
        step = 1
        num = (high - low)//step + 1
        count = low
        buff = 0
        for index in range(num):
            #Check effective packet
            while(self.CSI.Get_Bfee_count(count) == buff):
                #Not found Check_effection(Packet_Number)
                count = count + 1
            #Check
            if(self.CSI.Check_effection(count)):
                    print("Packet "+str(count)+" not found")
                    return 1
            Bfee_count =self.CSI.Get_Bfee_count(count)
            buff = Bfee_count
            CSI_result = self.CSI.Get_CSI(count)
            
            for subcarrier in range(0,180,6):
                point1.append(abs(CSI_result[subcarrier]))
                point2.append(abs(CSI_result[subcarrier+1]))
                point3.append(abs(CSI_result[subcarrier+2]))
                point4.append(abs(CSI_result[subcarrier+3]))
                point5.append(abs(CSI_result[subcarrier+4]))
                point6.append(abs(CSI_result[subcarrier+5]))
            plt.cla()
            plt.pause(0.0001)
            plt.axis([1, 30, 0, 100])
            plt.title('packet:'+str(index + 1)+',Real packet:'+str(count)+',Bfee_count:'+str(Bfee_count))
            plt.xlabel('subcarrier')
            plt.ylabel('amplitude(db)') 
            plt.plot(time,point1,'b')
            plt.plot(time,point2,'b')
            plt.plot(time,point3,'g')
            plt.plot(time,point4,'g')
            plt.plot(time,point5,'r')
            plt.plot(time,point6,'r')
            plt.pause(speed)
            del point1[:]
            del point2[:]
            del point3[:]
            del point4[:]
            del point5[:]
            del point6[:]
        plt.show()