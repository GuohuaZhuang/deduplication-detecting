
import java.lang.Long;
import java.math.BigInteger;

/**
 * 网络舆情预警计算机制_文档聚合度方法
 * 
 * 在外面控制聚合度对应不同预警等级的判断。目前无法给出具体值。
因为对预警等级的不确定，所以在给定预警时，可以参照如下示例配置：

	0 < poly_degree_variance < 20 	====> 正常无预警
	20 < poly_degree_variance < 40 	====> 普通预警
	40 < poly_degree_variance		====> 高危预警
 * 
 */

public class HammingDist
{
	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		// invoke mode1, using long array
		long[] arr = {0xe3, 0x01, 0x78, 0xaa};
		
		double value = poly_degree_variance(arr);// or poly_degree_statistic
		System.out.println("value = " + value);
		
		// invoke mode2, using hashcode string array
		String[] sarr = {"e09427bab793e836", "05fc7fd3b7282d75", "f35fe2cf7fbdffed"};
		long[] larr = {0, 0, 0};
		
		int len = sarr.length;
		int i = 0;
		for (i = 0; i < len; i ++)
		{
			larr[i] = (new BigInteger(sarr[i], 16)).longValue();
			// System.out.println("larr[" + i + "] = " + larr[i]);
		}
		
		double svalue = poly_degree_statistic(larr, 15); // or poly_degree_variance
		System.out.println("value = " + svalue);
	}
	
	/**
	 * Calculates the hamming distance.
	 */
	public static long hamming_dist(long a1, long a2)
	{
		long v1 = a1^a2;
		long v2 = (a1^a2)>>32;
		
		v1 = v1 - ((v1>>1) & 0x55555555);
		v2 = v2 - ((v2>>1) & 0x55555555);
		v1 = (v1 & 0x33333333) + ((v1>>2) & 0x33333333);
		v2 = (v2 & 0x33333333) + ((v2>>2) & 0x33333333);
		long c1 = (((v1 + (v1>>4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
		long c2 = (((v2 + (v2>>4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
		
		return c1+c2;
	}
	
	/**
	 * Calculates polymerization degree via variance.
	 * 方式１，传送long数组，long数组里存放的是64bit的hashcode值。
	 */
	public static double poly_degree_variance(long[] arr)
	{
		int len = arr.length;
		int i = 0, j = 0;
		double dist = 0.0;
		for (i = 0; i < len; i ++)
		{
			double one_dist = 0.0;
			for(j = 0; j < len; j ++)
			{
				one_dist += hamming_dist(arr[i], arr[j]);
			}
			one_dist /= len;
			dist += (one_dist * one_dist);
		}
		dist /= len;
		dist = Math.sqrt(dist);
		
		return dist;
	}
	
	/**
	 * Calculates polymerization degree via statistic.
	 * 方式２，传送string数组，要预先转化为long数值。
	 */
	public static double poly_degree_statistic(long[] arr, long kdist)
	{
		int len = arr.length;
		int i = 0, j = 0;
		int count = 0;
		for (i = 0; i < len; i ++)
		{
			double one_dist = 0.0;
			for(j = 0; j < len; j ++)
			{
				if (hamming_dist(arr[i], arr[j]) >= kdist)
				{
					count ++;
				}
			}
		}
		
		return (double)count/len;
	}
}

