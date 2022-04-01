#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int seqno = 0;
int n_designs = 0;

/* used in singly-linked list */
struct node
{
    int value;
    struct node* next;
};

/* add node to head of list */
static void
add_head(struct node** head_ref, int value)
{
    struct node* new_node = NULL;
    new_node = (struct node*)malloc(sizeof(struct node));

    if (new_node == NULL)
        printf("Unable to create node\n");

    new_node->value = value;
    new_node->next = *head_ref;
    *head_ref = new_node;
}

/* return mininimum of a and b */
int
min(int a, int b)
{
    if (b < a)
        return b;
    else
        return a;
}

/* Find unique combinations of size 2 and size 3 tiles to
 * make up a strip of length 30.
 * strips_sums: contains all such combinations
 * under the form of partial sums.
 * for example, 2-2-3 becomes 2-4-7
 * seq: current tile sequence
 * (a full sequence is a strip)
 * seq_pos: position in tile sequence
 * r_x: remaining number of size 2 tiles
 * r_y: remaining number of size 3 tiles
 */
static void
find_unique_strips(int* strips_sums[], int seq[], int seq_pos, int r_x, int r_y)
{
    /* if no tiles remain */
    if (r_x == 0 && r_y == 0)
    {
        strips_sums[seqno] = (int*)malloc(30 * sizeof(int));

        if (strips_sums[seqno] == NULL)
            printf("memory allocation failed\n");

        /* add tile sequence found to unique strips
         * (under the form of partial sums)
         */
        strips_sums[seqno][0] = seq[0];
        for (int i = 1; i < 30; i++)
            strips_sums[seqno][i] = strips_sums[seqno][i - 1] + seq[i];

        /* increment seq number because this one has just been used */
        seqno++;
    }

    /* try adding a tile of size 2 */
    if (r_x != 0)
    {
        seq[seq_pos] = 2;
        find_unique_strips(strips_sums, seq, seq_pos + 1, r_x - 1, r_y);
    }

    /* try adding a tile of size 3 */
    if (r_y != 0)
    {
        seq[seq_pos] = 3;
        find_unique_strips(strips_sums, seq, seq_pos + 1, r_x, r_y - 1);
    }
}

/* check if 2 strips share an inner edge */
static bool share_inner_edge(int* strips_sums[], int seqno1, int seqno2)
{
    for (int i = 0; i < 30; i++)
    {
        /* sequence 1 has been read already */
        if (strips_sums[seqno1][i] == 0 || strips_sums[seqno1][i] == 30)
        {
            break;
        }

        /* compare partial sums of both sequences
         * 2 sums being equal means tile share inner edge
         */
        for (int j = 0; j < 30; j++)
        {
            if (strips_sums[seqno1][i] == strips_sums[seqno2][j])
            {
                /* incompatible strips share inner edges */
                return true;
            }
            if (strips_sums[seqno2][j] == 0 || strips_sums[seqno2][j] == 30)
            {
                break;
            }
        }
    }

    /* compatible strips don't share inner edges */
    return false;
}

/* for each stripe, store a list of all the stripes it is
 * compatible with
 */
static void
create_compatibility_lists(int* strips_sums[], struct node** compatibilities_ref, int curr_seq)
{
    for (int j = 0; j < seqno; j++)
    {
        /* compatible stripe found
         * check partial sums of (curr_seq) and (j) stripes
         */
        if (!share_inner_edge(strips_sums, curr_seq, j))
        {
            /* add to list of stripes compatible with stripe curr_seq
             * new node is of seq number = j
             */
            add_head(compatibilities_ref, j);
        }
    }
}

/* compute number of designs of 11 stripes
 * that don't share inner edges
 * compatibilities: array of lists of compatibles stripes
 * cur_seqno: sequence number of the current strip
 * depth: number of strips selected so far
 */
static void
compute_designs(struct node* compatibilities[], int cur_seqno, int depth)
{
    /* max depth reached */
    if (depth == 11)
    {
        n_designs++;
        return;
    }

    /* pick a strip among strips compatible
     * with the current strip
     */
    struct node* curr = compatibilities[cur_seqno];
    while (curr != NULL)
    {
        compute_designs(compatibilities, curr->value, depth + 1);
        curr = curr->next;
    }
}

void bench_main()
{
    /* strip of length 30 contains at most 30 tiles */
    int max_tiles = 30;
    int x[max_tiles];
    int y[max_tiles];
    int index = 0;
    int sum = 0;

    /* Find all couples (x, y) such that there can be x tiles of size 2
     * and y tiles of size 3 in a strip of length 30
     * 2x + 3y = 30
     */
    for (int i = 0; i < 30; i++)
    {
        /* strip length can't exceed 30 */
        if (2 * i > 30)
        {
            continue;
        }

        for (int j = 0; j < 30; j++)
        {
            sum = 2 * i + 3 * j;

            /* couple found */
            if (sum == 30)
            {
                x[index] = i;
                y[index] = j;
                index++;
                break;

                /* strip too long */
            }

        	if (sum > 30)
            {
                break;
            }
        }
    }

    /* Print solutions */
    for (int i = 0; i < index; i++)
    {
        printf("Found 2*%d + 3*%d = 30 \n", x[i], y[i]);
    }

    /* Note: there are index couples that are solution
     * to the equation: 2x + 3y = 30
     */

     /* Find all unique possibilities for a strip of length 30 */
    int max_poss = 1000;
    /* partial sums in design */
    int* strips_sums[max_poss];

    int seq[30] = { 0 };

    /* For each couple (x, y)
     */
    for (int i = 0; i < index; i++)
    {
        /* Find unique possibilites for strip */
        find_unique_strips(strips_sums, seq, 0, x[i], y[i]);
    }

    /* uncomment to print all possible strips */
    /*
    printf("\nall possible strips:\n");
    for (int i = 0; i < seqno; i++)
    {
        printf("i = %d :", i);
        for (int j = 0; j < 30; j++)
            printf("%d ", strips_sums[i][j]);
        printf("\n");
    }
    */

    /* for each strip, store list of compatible strips
     * compatibilites[11] = 2 -> 7 -> 9 -> 15 (singly-linked list)
     */
    struct node* compatibilities[seqno];
    for (int i = 0; i < seqno; i++)
    {
        compatibilities[i] = NULL;
        create_compatibility_lists(strips_sums, &compatibilities[i], i);
    }
    printf("created compatibility lists\n");


    /* Find all designs for a parquet of 11 strips where
     * 2 adjacent strips can't share an inner edge
     */
     /* get all designs starting with 1, then 2, then 3... */
    for (int i = 0; i < seqno; i++)
    {
        compute_designs(compatibilities, i, 1);
    }

    printf("%d possible designs found", n_designs);

    /* Free memory */
    struct node* next;
    struct node* curr;
    for (int j = 0; j < seqno; j++)
    {
        free(strips_sums[j]);

        curr = compatibilities[j];
        while (curr != NULL)
        {
            next = curr->next;
            free(curr);
            curr = next;
        }
    }

    printf("\ndone");
}