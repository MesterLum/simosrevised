# Revised Simos Algorithm: 10 Real-World Test Scenarios

This document contains 10 standardized test scenarios to verify the logic of the Revised Simos algorithm. Each scenario represents a real-world decision-making problem (e.g., buying a car, hiring employees) to make the math easier to visualize.

---

## 1. Budget Gaming Laptop

**The Story:** A student is buying a budget laptop for gaming. The price is the limiting factor (most important). The graphics card (GPU) is the second priority for performance. RAM is third because it can be upgraded later. RGB lights are just cosmetic and least important.

- **Importance Ratio (z):** 5.0 (Price is 5x more important than RGB Lights)
- **Precision:** 2 Decimals
- **White Cards (Gaps):** None

**Expected Weights:**

| Rank | Criteria   | Weight (%) |
| ---- | ---------- | ---------- |
| 1    | RGB Lights | 8.33       |
| 2    | RAM        | 19.44      |
| 3    | GPU Power  | 30.56      |
| 4    | Price      | 41.67      |

**Total Sum:** 100.00

---

## 2. Apartment Hunting (Safety First)

**The Story:** A person is looking for an apartment in a new city. Safety is non-negotiable and vastly more important than any luxury feature. Rent is the next practical concern. The View and Pool are nice-to-haves but very low priority.

- **Importance Ratio (z):** 20.0
- **Precision:** 1 Decimal
- **White Cards (Gaps):** 4 White Cards inserted between "Rent" and "Safety" to create a massive jump in importance.

**Expected Weights:**

| Rank | Criteria | Weight (%) |
| ---- | -------- | ---------- |
| 1    | Pool     | 2.6        |
| 2    | View     | 8.8        |
| 3    | Rent     | 15.1       |
| 4    | Safety   | 73.5       |

**Total Sum:** 100.0

---

## 3. Hiring a Junior Developer

**The Story:** A tech company is hiring a Junior Developer. Since it is a junior role, they care most about long-term Potential. Coding Basics are second. A University Degree is third. Location is last. Because it is an entry-level job, the difference between the most important and least important traits is small.

- **Importance Ratio (z):** 2.5
- **Precision:** 3 Decimals
- **White Cards (Gaps):** None

**Expected Weights:**

| Rank | Criteria      | Weight (%) |
| ---- | ------------- | ---------- |
| 1    | Location      | 14.286     |
| 2    | Degree        | 21.429     |
| 3    | Coding Basics | 28.571     |
| 4    | Potential     | 35.714     |

**Total Sum:** 100.000

---

## 4. Critical Database Selection

**The Story:** A bank is choosing a database system. Data Integrity is the only thing that matters; if data is lost, the bank fails. Speed, Cost, and Ease of Use are secondary concerns.

- **Importance Ratio (z):** 8.0
- **Precision:** 0 Decimals (Integers only)
- **White Cards (Gaps):** None

**Expected Weights:**

| Rank | Criteria       | Weight (%) |
| ---- | -------------- | ---------- |
| 1    | Ease of Use    | 6          |
| 2    | Cost           | 19         |
| 3    | Speed          | 31         |
| 4    | Data Integrity | 44         |

**Total Sum:** 100

---

## 5. Emergency Room Triage

**The Story:** A hospital triage system prioritizing patients. Vital Signs are life-or-death. Pain Level is urgent. Insurance and Wait Time are barely considered compared to saving a life.

- **Importance Ratio (z):** 100.0 (Vitals are 100x more important than Wait Time)
- **Precision:** 2 Decimals
- **White Cards (Gaps):** None

**Expected Weights:**

| Rank | Criteria   | Weight (%) |
| ---- | ---------- | ---------- |
| 1    | Wait Time  | 0.50       |
| 2    | Insurance  | 16.83      |
| 3    | Pain Level | 33.17      |
| 4    | Vitals     | 49.50      |

**Total Sum:** 100.00

---

## 6. Family Car (Utility vs. Vanity)

**The Story:** A family needs a car. Safety and Space are in the "Utility" tier (very important). Brand and Color are in the "Vanity" tier (low importance). There is a clear separation between the necessary features and the cosmetic ones.

- **Importance Ratio (z):** 6.0
- **Precision:** 4 Decimals
- **White Cards (Gaps):** 1 White Card placed between "Brand" (Rank 2) and "Space" (Rank 3) to separate the two tiers.

**Expected Weights:**

| Rank | Criteria | Weight (%) |
| ---- | -------- | ---------- |
| 1    | Color    | 7.1429     |
| 2    | Brand    | 16.0714    |
| 3    | Space    | 33.9286    |
| 4    | Safety   | 42.8571    |

**Total Sum:** 100.0000

---

## 7. Restaurant Choice

**The Story:** Choosing a place for dinner. The Taste of the food is the most important factor. Hygiene is second. Distance from home is third. The Decor (interior design) is least important.

- **Importance Ratio (z):** 4.0
- **Precision:** 1 Decimal
- **White Cards (Gaps):** None

**Expected Weights:**

| Rank | Criteria | Weight (%) |
| ---- | -------- | ---------- |
| 1    | Decor    | 10.0       |
| 2    | Distance | 20.0       |
| 3    | Hygiene  | 30.0       |
| 4    | Taste    | 40.0       |

**Total Sum:** 100.0

---

## 8. Software Compliance (The "Force Up" Case)

**The Story:** A company buying software. Compliance (Legal) is the top priority, followed by Support, Features, and User Interface (UI). This specific mathematical scenario forces the algorithm to round numbers up to meet the 100% total.

- **Importance Ratio (z):** 3.5
- **Precision:** 2 Decimals
- **White Cards (Gaps):** None

**Expected Weights:**

| Rank | Criteria            | Weight (%) |
| ---- | ------------------- | ---------- |
| 1    | User Interface (UI) | 11.11      |
| 2    | Features            | 20.37      |
| 3    | Support             | 29.63      |
| 4    | Compliance          | 38.89      |

**Total Sum:** 100.00

---

## 9. Smartphone (The Big Jump)

**The Story:** A user wants a phone with a great Battery and Camera. They do not care about the Brand or the Speaker quality. There is a massive gap in importance between the top features and the bottom features.

- **Importance Ratio (z):** 12.0
- **Precision:** 3 Decimals
- **White Cards (Gaps):** 3 White Cards placed between "Brand" (Rank 2) and "Camera" (Rank 3).

**Expected Weights:**

| Rank | Criteria | Weight (%) |
| ---- | -------- | ---------- |
| 1    | Speaker  | 2.653      |
| 2    | Brand    | 6.897      |
| 3    | Camera   | 28.117     |
| 4    | Battery  | 62.333     |

**Total Sum:** 100.000

---

## 10. University Selection (Tie Break)

**The Story:** A student is choosing a university. All criteria are almost equally important. Reputation is slightly better than Cost, which is slightly better than Campus, which is slightly better than Weather.

- **Importance Ratio (z):** 1.5 (Very flat hierarchy)
- **Precision:** 2 Decimals
- **White Cards (Gaps):** None

**Expected Weights:**

| Rank | Criteria   | Weight (%) |
| ---- | ---------- | ---------- |
| 1    | Weather    | 20.00      |
| 2    | Campus     | 23.33      |
| 3    | Cost       | 26.67      |
| 4    | Reputation | 30.00      |

**Total Sum:** 100.00
