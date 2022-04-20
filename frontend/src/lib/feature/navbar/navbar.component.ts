import { Component, EventEmitter, Input, Output } from "@angular/core";
import { List, App } from "src/lib/data-access/models";
import { BackendService } from 'src/lib/data-access/service/backend.service';

@Component({
    selector: 'navbar',
    templateUrl: './navbar.component.html',
    styleUrls: ['./navbar.component.scss'],
})
export class NavbarComponent {
    @Input() app: App;
    @Output() clickDeleteEvent = new EventEmitter<number>();

    constructor( private backendService: BackendService ) { }

    onListCreate(): void {
        let newList: List = { title: '', items: [], selected: false, itemCounter: 0 };
        this.app.lists.push(newList);
        this.backendService.postList(newList).subscribe( (list) => {
          newList.id = list.id;
        });
    }

    onListDelete(id: number): void {
        this.clickDeleteEvent.emit(id);
    }

    onListSelect(event: List): void {
        if (this.app.selectedList != null && this.app.selectedList.id != event.id) {
            this.app.selectedList.selected = false;
        }
        this.app.selectedList = event;
    }

    onTodayList(): void {
        // so that changes to the items in the filtered lists are also applied to the normal lists,
        // the items are not queried by the backend but filtered out
        let todayList: List = { id: -1, title: 'Today', items: [], selected: true, itemCounter: 0 };
        for (let i = 0; i < this.app.lists.length; i++) {
          for (let j = 0; j < this.app.lists[i].items.length; j++) {
            if (this.isToday(this.app.lists[i].items[j].date)) {
              todayList['items'].push(this.app.lists[i].items[j]);
            }
          }
        }
        todayList['itemCounter'] = todayList['items'].length;
        if (this.app.selectedList) this.app.selectedList.selected = false;
        this.app.selectedList = todayList;
    }

    onFlaggedList(): void {
        // so that changes to the items in the filtered lists are also applied to the normal lists,
        // the items are not queried by the backend but filtered out

        let flaggedList: List = { id: -1, title: 'Flagged', items: [], selected: true, itemCounter: 0 };
        for (let i = 0; i < this.app.lists.length; i++) {
          for (let j = 0; j < this.app.lists[i].items.length; j++) {
            if (this.app.lists[i].items[j].flagged) {
              flaggedList['items'].push(this.app.lists[i].items[j]);
            }
          }
        }
        flaggedList['itemCounter'] = flaggedList['items'].length;
        if (this.app.selectedList) this.app.selectedList.selected = false;
        this.app.selectedList = flaggedList;
    }

    isToday (someDate: string): boolean {
        const today: string = new Date().toISOString().substring(0, 10);
        return today == someDate;
    }
}
