import { List, Item, App } from 'src/lib/data-access/models';
import { Component, ElementRef, EventEmitter, Input, Output, ViewChild } from "@angular/core";
import { BackendService } from 'src/lib/data-access/service/backend.service';

@Component({
    selector: 'list',
    templateUrl: './list.component.html',
    styleUrls: ['./list.component.scss'],
})
export class ListComponent {
    @Input() app: App;
    @Input() list: List;
    @Output() clickDeleteEvent = new EventEmitter<number>();
    @Output() itemFlaggedEvent = new EventEmitter<boolean>();

    @ViewChild('listNameInput') myInput: ElementRef;

    selectedItemID: number;

    showTrash: boolean = false;

    constructor( private backendService: BackendService ) { }

    onMouseEnter(): void {
        this.showTrash = true;
    }

    onMouseLeave(): void {
        this.showTrash = false;
    }

    onTrashEvent(): void {
        this.clickDeleteEvent.emit(this.list.id);
    }

    onItemCreate(): void {
        let newItem: Item = { text: '', flagged: false };
        this.list.itemCounter ++;
        this.list.items.push(newItem);
        this.backendService.postItem(this.list.id, newItem).subscribe( (item) => {
            newItem.id = item.id;
        });
    }


    onItemFlagged(event: number): void {
        for (let i = 0; i < this.list.items.length; i++) {
            if (this.list.items[i].id == event){
              if (this.list.id == -1) {
                  this.list.items.splice(i, 1);
                  this.app.flaggedCount --;
              } else {
                  this.itemFlaggedEvent.emit(this.list.items[i].flagged);
              }
            }
        }
    }

    onTitleChange(event: any): void {
        this.list.title = event.target.value;
        this.backendService.putList(this.list).subscribe();
    }

    onItemDelete(id: number): void {
        const index = this.list.items.findIndex(
            (list) => list.id === id
        );
        let item = this.list.items[index];

        // update filter counter
        if (item.flagged) {
            this.app.flaggedCount --;
        }
        if (this.isToday(item.date)) {
            this.app.todayCount --;
        }

        // if we are in a FilteredList, the data must also be adjusted from the actual list
        if (this.list.id == -1) {
          for (let i = 0; i < this.app.lists.length; i++) {
            for (let j = 0; j < this.app.lists[i].items.length; j++) {
              if (this.app.lists[i].items[j].id == id && this.app.lists[i].id != -1) {
                this.app.lists[i].itemCounter --;
                this.app.lists[i].items.splice(j, 1);
                break;
              }
            }
          }
        }

        this.list.items.splice(index, 1);
        this.list.itemCounter --;

        this.backendService.deleteItem(id).subscribe();
    }

    onItemDateChangedEvent(id: number): void {
        // If a date in the Today list is set to a different date, the item must be removed from the Today list
        if (this.list.id == -1 && this.list.title == "Today") {
            for (let i = 0; i < this.list.items.length; i++) {
                if (this.list.items[i].id === id) {
                    this.list.items.splice(i, 1);
                }
            }
        }
    }

    onUpdateTodayCounterEvent(event: number): void {
        this.app.todayCount += event;
    }

    getValue(): string {
        return this.list.title;
    }

    getAnzItems(): number {
        return this.list.items.length;
    }

    isToday (someDate: string): boolean {
        const today: string = new Date().toISOString().substring(0, 10);
        return today == someDate;
    }
}
